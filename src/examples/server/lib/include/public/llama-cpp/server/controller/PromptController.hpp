#ifndef A6B0E536_F2E4_4396_A703_FBD65CDEAA66
#define A6B0E536_F2E4_4396_A703_FBD65CDEAA66

#include <cctype>
#include <cstddef>
#include <iomanip>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <llama-cpp/server/dto/PromptDto.hpp>
#include <llama-cpp/server/dto/StatusDto.hpp>

#include <llama-cpp/server/controller/SwaggerApiController.hpp>
#include <llama-cpp/server/services/ChatService.hpp>

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

class PromptController : public SwaggerApiController {
private:
  ChatService _chatService;

public:
  PromptController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : SwaggerApiController(objectMapper) {}

public:
  static const std::shared_ptr<PromptController> createShared(OATPP_COMPONENT(
      std::shared_ptr<ObjectMapper>,
      objectMapper) // Inject objectMapper component here as default parameter
  ) {
    return std::make_shared<PromptController>(objectMapper);
  }

  ENDPOINT_INFO(postPromptMessage) {
    info->summary = "Post the chat meesage.";

    info->addConsumes<Object<PromptDto>>("application/json");

    info->addResponse<Object<PromptDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT("POST", "/api/prompt", postPromptMessage,
           BODY_DTO(Object<PromptDto>, promptDto)) {
    const auto &response = this->_chatService.predict(promptDto);
    return createDtoResponse(Status::CODE_200, response);
  }

  ENDPOINT_INFO(getStaticResponse) {
    info->summary = "Get static response for testing purpose.";

    info->addResponse<String>(Status::CODE_200, "application/text");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT("GET", "/api/prompt", getStaticResponse) {
    return createResponse(Status::CODE_200, String("Hello from LLama server!"));
  }

  ENDPOINT_INFO(getParameters) {
    info->summary = "Get parameters.";

    info->addResponse<Object<ParamsDto>>(Status::CODE_200, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_404, "application/json");
    info->addResponse<Object<StatusDto>>(Status::CODE_500, "application/json");
  }
  ENDPOINT("GET", "/api/prompt/parameters", getParameters) {
    const auto &params = this->_chatService.get_params();
    return createDtoResponse(Status::CODE_200, params);
  }
};

#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif /* A6B0E536_F2E4_4396_A703_FBD65CDEAA66 */
