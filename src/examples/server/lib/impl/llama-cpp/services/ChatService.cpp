#include <filesystem>
#include <functional>
#include <iterator>
#include <mutex>
#include <string>

#include <llama-cpp/llama.hpp>
#include <llama-cpp/utils.hpp>

#include <llama-cpp/server/dto/PromptDto.hpp>

#include <llama-cpp/server/services/ChatService.hpp>

ChatService::ChatService() : ChatService("./models") {}

ChatService::ChatService(const std::string &path) {
  this->scan_for_models(path);
}

ChatService::~ChatService() {}

void ChatService::scan_for_models(const std::string &path) {
  const std::lock_guard<std::mutex> lock(this->_models_mtx);
  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    this->_models.emplace(entry.path().filename().string(),
                          entry.path().string());
  }
}

std::unordered_map<std::string, std::string>
ChatService::get_model_list() const {
  return this->_models;
}

std::string ChatService::map_model_path(const std::string &name) const {
  const auto &item = this->_models.find(name);
  if (item == this->_models.end()) {
    throw std::runtime_error("Model not found");
  }
  return llama_cpp::internal::filesystem::normalize_path(item->second);
}

std::size_t ChatService::get_threads_count(std::size_t count) const {
  const auto &hardware_threads =
      llama_cpp::internal::threading::get_processor_count();
  if (count == 0 || count > hardware_threads) {
    count = hardware_threads;
  }
  return count;
}

oatpp::Object<ParamsDto> ChatService::get_params() const {
  const auto &models = this->get_model_list();

  auto dto = ParamsDto::createShared();

  dto->max_threads_count = this->get_threads_count();
  dto->models = oatpp::Vector<oatpp::String>::createShared();

  for (const auto &m : models) {
    dto->models->push_back(m.first);
  }

  return dto;
}

oatpp::Object<PromptDto>
ChatService::predict(const oatpp::Object<PromptDto> &dto) {
  const std::string session_id = dto->session_id;

  auto prompt = dto->prompt;
  auto model = this->map_model_path(dto->model);

  auto temperature = dto->temperature.getValue(0.8);
  auto n_predict = dto->n_predict.getValue(-1);
  auto n_ctx = dto->n_ctx.getValue(512);
  auto n_keep = dto->n_keep.getValue(0);

  auto top_p = dto->top_p.getValue(0.95);
  auto top_k = dto->top_k.getValue(40);
  auto ignore_eos = dto->ignore_eos.getValue(true);
  auto repeat_last_n = dto->repeat_last_n.getValue(64);
  auto batch_size = dto->batch_size.getValue(512);
  auto threads = this->get_threads_count(dto->threads.getValue(0));

  auto frequency_penalty = dto->frequency_penalty.getValue(0);
  auto presence_penalty = dto->presence_penalty.getValue(0);
  auto repeat_penalty = dto->repeat_penalty.getValue(1.1);

  std::vector<std::string> stop_prompts(dto->stop_prompts->begin(),
                                        dto->stop_prompts->end());

  session_context session;

  this->_sessions.lazy_emplace_l(
      session_id,
      [&](session_map::value_type &p) {
        session = p.second;
      }, // called only when key was already present
      [&](const session_map::constructor
              &ctor) // construct value_type in place when key not present
      {
        const auto &new_session_ptr =
            new llama_cpp::context(model, {[&](llama_cpp::model_options &opts) {
                                     opts.context_size = n_ctx;
                                     return opts;
                                   }});
        std::shared_ptr<llama_cpp::context> new_session(new_session_ptr);
        session = new_session;
        session->load();

        session->set_predict_options({[&](llama_cpp::predict_options &po) {
#if _DEBUG
          po.debug_mode = true;
#endif

          po.n_ctx = n_ctx;
          po.ignore_eos = ignore_eos;
          po.penalty = repeat_penalty;
          po.tokens = n_predict;
          po.temperature = temperature;
          po.top_k = top_k;
          po.top_p = top_p;
          po.n_keep = n_keep;
          po.repeat = repeat_last_n;
          po.batch = batch_size;
          po.threads = (std::int32_t)threads;
          po.stop_prompts = stop_prompts;
          return po;
        }});

        ctor(session_id, new_session);
      });

  const auto &value = session->predict(prompt);

  auto response = PromptDto::createShared();
  response->session_id = session_id;
  response->prompt = prompt;
  response->bot = value;
  response->model = model;

  return response;
}