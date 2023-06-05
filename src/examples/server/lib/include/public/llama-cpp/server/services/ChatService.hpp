#ifndef C8B4AC0C_3CD2_4707_97D1_F4DD9D81C330
#define C8B4AC0C_3CD2_4707_97D1_F4DD9D81C330

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include <parallel_hashmap/phmap.h>

#include <llama-cpp/llama.hpp>

#include <llama-cpp/server/dto/ParamsDto.hpp>
#include <llama-cpp/server/dto/PromptDto.hpp>


#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

class ChatService {
  using session_context = std::shared_ptr<llama_cpp::context>;
  using session_map =
      phmap::parallel_flat_hash_map_m<std::string, session_context>;

public:
  ChatService();
  ChatService(const std::string &path);
  ~ChatService();

public:
  std::unordered_map<std::string, std::string> get_model_list() const;

  oatpp::Object<ParamsDto> get_params() const;

  oatpp::Object<PromptDto> predict(const oatpp::Object<PromptDto> &dto);

protected:
  void scan_for_models(const std::string &dir);

  std::string map_model_path(const std::string &name) const;

  std::size_t get_threads_count(std::size_t count = 0) const;

protected:
  session_map _sessions;
  std::unordered_map<std::string, std::string> _models;
  std::mutex _models_mtx;
};

#endif /* C8B4AC0C_3CD2_4707_97D1_F4DD9D81C330 */
