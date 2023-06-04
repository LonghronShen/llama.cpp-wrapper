#include <llama-cpp/llama.hpp>

#include <llama-cpp/server/dto/PromptDto.hpp>

#include <llama-cpp/server/services/ChatService.hpp>

ChatService::ChatService() {}

ChatService::~ChatService() {}

oatpp::Object<PromptDto>
ChatService::predict(const oatpp::Object<PromptDto> &dto) {
  const std::string session_id = dto->session_id;

  auto prompt = dto->prompt;
  auto bot = dto->bot;
  auto model = dto->model;
  auto temperature = dto->temperature;
  auto max_tokens = dto->max_tokens;
  auto top_p = dto->top_p;
  auto frequency_penalty = dto->frequency_penalty;
  auto presence_penalty = dto->presence_penalty;

  this->_sessions.lazy_emplace_l(
      session_id,
      [&](session_map::value_type &p) {
        ;
      }, // called only when key was already present
      [&](const session_map::constructor
              &ctor) // construct value_type in place when key not present
      {
        const auto &session = std::make_shared<llama_cpp::context>();
        ctor(std::move(session_id), session);
      });

  auto response = PromptDto::createShared();
  response->prompt = prompt;
  response->bot = bot;
  response->model = model;
  response->temperature = temperature;
  response->max_tokens = max_tokens;
  response->top_p = top_p;
  response->frequency_penalty = frequency_penalty;
  response->presence_penalty = presence_penalty;

  return response;
}