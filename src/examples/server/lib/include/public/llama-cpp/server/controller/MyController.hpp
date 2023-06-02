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
#include <unordered_map>
#include <vector>

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/parser/json/mapping/ObjectMapper.hpp>
#include <oatpp/web/server/api/ApiController.hpp>

#include <llama-cpp/server/dto/MyDTOs.hpp>

#include <cmrc/cmrc.hpp>

CMRC_DECLARE(llama_cpp::server::rc);

class MyController : public oatpp::web::server::api::ApiController {
protected:
  MyController(const std::shared_ptr<ObjectMapper> &objectMapper)
      : oatpp::web::server::api::ApiController(objectMapper) {}

public:
  /**
   *  Inject @objectMapper component here as default parameter
   *  Do not return bare Controllable* object! use shared_ptr!
   */
  static std::shared_ptr<MyController>
  createShared(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)) {
    return std::shared_ptr<MyController>(new MyController(objectMapper));
  }

  /**
   *  Begin ENDPOINTs generation ('ApiController' codegen)
   */
#include OATPP_CODEGEN_BEGIN(ApiController)

  /**
   *  Root endpoint Coroutine mapped to the "/" (root)
   */
  ENDPOINT_ASYNC("GET", "/", Root) {

    ENDPOINT_ASYNC_INIT(Root)

    /**
     *  Coroutine entrypoint act()
     *  returns Action (what to do next)
     */
    Action act() override {
      auto fs = cmrc::llama_cpp::server::rc::get_filesystem();
      auto file = fs.open("index.html");
      std::string buffer(file.begin(), file.end());
      auto response =
          controller->createResponse(Status::CODE_200, buffer.c_str());
      response->putHeader("content-type", "text/html; charset=UTF-8");
      return _return(response);
    };
  };

  // ENDPOINT_ASYNC("POST", "/api/map_concept/gaia/{version}",
  //                get_gaia_concept_node) {

  //   ENDPOINT_ASYNC_INIT(get_gaia_concept_node)

  //   Action act() override {
  //     auto version_str = request->getPathVariables().getVariable("version");
  //     auto version = version_str->c_str();
  //     auto body = request->readBodyToString();

  //     try {
  //       auto [text, condition_json] = get_body_info(body);

  //       auto result =
  //           get_concept_node(text.c_str(), condition_json.c_str(), version);
  //       std::string response_json = result;
  //       free_char(result);

  //       auto response =
  //           controller->createResponse(Status::CODE_200, response_json.c_str());
  //       response->putHeader("content-type", "application/json; charset=UTF-8");
  //       return _return(response);
  //     } catch (const std::exception &e) {
  //       return _return(controller->createResponse(Status::CODE_500, e.what()));
  //     }
  //   };
  // };

#include OATPP_CODEGEN_END(ApiController)
};
#endif /* A6B0E536_F2E4_4396_A703_FBD65CDEAA66 */
