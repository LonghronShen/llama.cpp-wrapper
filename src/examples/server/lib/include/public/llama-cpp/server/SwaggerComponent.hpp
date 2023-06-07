#ifndef A1118E9F_AA17_4AC7_ACCC_551BC3E270B2
#define A1118E9F_AA17_4AC7_ACCC_551BC3E270B2

#include <memory>

#include <oatpp-swagger/EmbeddedResources.hpp>
#include <oatpp-swagger/Model.hpp>

#include <oatpp/core/macro/component.hpp>

/**
 *  Swagger ui is served at
 *  http://host:port/swagger/ui
 */
class SwaggerComponent {
public:
  /**
   *  General API docs info
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>,
                         swaggerDocumentInfo)
  ([] {
    oatpp::swagger::DocumentInfo::Builder builder;

    builder.setTitle("User entity service")
        .setDescription("LLama.cpp web server project with swagger docs")
        .setVersion("1.0")
        .setContactName("LonghronShen")
        .setContactUrl("https://oatpp.io/")

        .setLicenseName("Apache License, Version 2.0")
        .setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0");

        // .addServer("http://localhost:5000", "server on localhost");

    return builder.build();
  }());

  /**
   *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>,
                         swaggerResources)
  ([] {
    // Make sure to specify correct full path to oatpp-swagger/res folder !!!
    return std::make_shared<oatpp::swagger::EmbeddedResources>();
  }());
};

#endif /* A1118E9F_AA17_4AC7_ACCC_551BC3E270B2 */
