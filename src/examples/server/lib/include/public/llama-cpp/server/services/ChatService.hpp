#ifndef C8B4AC0C_3CD2_4707_97D1_F4DD9D81C330
#define C8B4AC0C_3CD2_4707_97D1_F4DD9D81C330

#include <llama-cpp/server/dto/PromptDto.hpp>

#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/protocol/http/Http.hpp"

class ChatService {
private:
  typedef oatpp::web::protocol::http::Status Status;

private:
public:
  oatpp::Object<PromptDto> createUser(const oatpp::Object<PromptDto> &dto);
};

#endif /* C8B4AC0C_3CD2_4707_97D1_F4DD9D81C330 */
