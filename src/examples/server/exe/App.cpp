#include <iostream>
#include <string>

#include "AppComponent.hpp"
#include "controller/MyController.hpp"

#include <oatpp/network/server/Server.hpp>

#include <synyi/common/logging/logger.h>
#include <synyi/common/system/interop/locale.h>
#include <synyi/map_concept/core/map_concept_wrapper.h>

std::string get_env_default(const std::string &env_name,
                            const std::string &defualt_value) {
  auto value = getenv(env_name.c_str());
  auto final = value == nullptr ? defualt_value : std::string(value);
  WriteLog<char>(log_level::info, "Envrionment Variable {0}: {1}", env_name,
                 final);
  return final;
}

/**
 *  run() method.
 *  1) set Environment components.
 *  2) add ApiController's endpoints to router
 *  3) run server
 */
void run() {
  std::string S3_ENDPOINT =
      get_env_default("S3_ENDPOINT", "172.16.127.100:39900");
  std::string S3_ACCESS_KEY =
      get_env_default("S3_ACCESS_KEY", "AKIAIOSFODNN7EXAMPLE");
  std::string S3_ACCESS_TOKEN = get_env_default(
      "S3_ACCESS_TOKEN", "wJalrXUtnFEMI/K7MDENG/bPxRfiCYEXAMPLEKEY");
  std::string S3_BUCKET_NAME =
      get_env_default("S3_BUCKET_NAME", "map-concept-data-pack-runtime");
  std::string S3_OBJECT_PATH = get_env_default("S3_OBJECT_PATH", "");
  std::string S3_REGION = get_env_default("S3_REGION", "us-east-1");

  set_s3_config(S3_ENDPOINT.c_str(), S3_ACCESS_KEY.c_str(),
                S3_ACCESS_TOKEN.c_str(), S3_BUCKET_NAME.c_str(),
                S3_OBJECT_PATH.c_str(), S3_REGION.c_str());

  AppComponent components; // Create scope Environment components

  /* create ApiControllers and add endpoints to router */

  auto router = components.httpRouter.getObject();

  auto myController = MyController::createShared();
  myController->addEndpointsToRouter(router);

  /* create server */

  oatpp::network::server::Server server(
      components.serverConnectionProvider.getObject(),
      components.serverConnectionHandler.getObject());

  OATPP_LOGD("Server", "Running on port %s...",
             components.serverConnectionProvider.getObject()
                 ->getProperty("port")
                 .toString()
                 ->c_str());

  server.run();
}

/**
 *  main
 */
int main(int argc, const char *argv[]) {
// SetConsoleOutputCodePageAsUTF8();
#if WIN32
  SetConsoleOutputCP(CP_UTF8);
#endif

  init_global_instance(false);

  oatpp::base::Environment::init();

  run();

  /* Print how much objects were created during app running, and what have
   * left-probably leaked */
  /* Disable object counting for release builds using '-D
   * OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount()
            << "\n";
  std::cout << "objectsCreated = "
            << oatpp::base::Environment::getObjectsCreated() << "\n\n";

  oatpp::base::Environment::destroy();

  return 0;
}
