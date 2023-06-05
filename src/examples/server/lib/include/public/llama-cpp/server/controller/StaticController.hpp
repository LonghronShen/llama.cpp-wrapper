#ifndef E1276BBD_C319_4ECB_9C70_5C4449F3AD35
#define E1276BBD_C319_4ECB_9C70_5C4449F3AD35

#include "mime-types/core.hpp"
#include <cctype>
#include <cstddef>
#include <iomanip>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <cmrc/cmrc.hpp>
#include <mime-types/mime_mapper.hpp>

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

CMRC_DECLARE(llama_cpp::server::rc);

#include OATPP_CODEGEN_BEGIN(ApiController) //<- Begin Codegen

class StaticController : public oatpp::web::server::api::ApiController {
public:
  StaticController(const std::shared_ptr<ObjectMapper> &objectMapper)
      : oatpp::web::server::api::ApiController(objectMapper) {}

public:
  static std::shared_ptr<StaticController> createShared(OATPP_COMPONENT(
      std::shared_ptr<ObjectMapper>,
      objectMapper) // Inject objectMapper component here as default parameter
  ) {
    return std::make_shared<StaticController>(objectMapper);
  }

  ENDPOINT("GET", "*", root,
           REQUEST(std::shared_ptr<IncomingRequest>, request)) {
    std::string requestPath = request->getPathTail()->c_str();
    if (requestPath == "") {
      requestPath = "index.html";
    }
    const std::string filePath = "dist/" + requestPath;
    auto fs = cmrc::llama_cpp::server::rc::get_filesystem();
    if (fs.exists(filePath)) {
      auto file = fs.open(filePath);
      std::string buffer(file.begin(), file.end());
      auto response = createResponse(Status::CODE_200, buffer.c_str());
      const auto &mime_type = mime_types::get_type(filePath);
      response->putHeader("content-type", mime_type);
      return response;
    }
    auto response = createResponse(Status::CODE_404);
    response->putHeader("content-type", "text/html; charset=UTF-8");
    return response;
  }
};

#include OATPP_CODEGEN_END(ApiController) //<- End Codegen

#endif /* E1276BBD_C319_4ECB_9C70_5C4449F3AD35 */
