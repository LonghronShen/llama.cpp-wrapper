#include <iostream>
#include <string>

#include <llama-cpp/server/AppComponent.hpp>

#include <llama-cpp/server/controller/PromptController.hpp>

#include <oatpp/network/Server.hpp>
#include <oatpp-swagger/Controller.hpp>

std::string get_env_default(const std::string &env_name,
                            const std::string &defualt_value) {
  auto value = getenv(env_name.c_str());
  auto final = value == nullptr ? defualt_value : std::string(value);
  return final;
}

void run() {
  AppComponent components; // Create scope Environment components

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  oatpp::web::server::api::Endpoints docEndpoints;

  docEndpoints.append(
      router->addController(PromptController::createShared())->getEndpoints());

  router->addController(oatpp::swagger::Controller::createShared(docEndpoints));

  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>,
                  connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
                  connectionProvider);

  /* create server */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  OATPP_LOGD("Server", "Running on port %s...",
             connectionProvider->getProperty("port").toString()->c_str());

  server.run();
}

/**
 *  main
 */
int main(int argc, const char *argv[]) {
  oatpp::base::Environment::init();

  run();

  /* Print how many objects were created during app running, and what have
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
