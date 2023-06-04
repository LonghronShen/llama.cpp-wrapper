#ifndef C8B4AC0C_3CD2_4707_97D1_F4DD9D81C330
#define C8B4AC0C_3CD2_4707_97D1_F4DD9D81C330

#include <memory>
#include <string>
#include <unordered_map>

#include <parallel_hashmap/phmap.h>

#include <llama-cpp/llama.hpp>

#include <llama-cpp/server/dto/PromptDto.hpp>

#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/Http.hpp>

class ChatService {
  using session_context = std::shared_ptr<llama_cpp::context>;
  using session_map = phmap::flat_hash_map<std::string, session_context>;

public:
  ChatService();
  ~ChatService();

public:
  oatpp::Object<PromptDto> predict(const oatpp::Object<PromptDto> &dto);

protected:
  session_map _sessions;
};

#endif /* C8B4AC0C_3CD2_4707_97D1_F4DD9D81C330 */
