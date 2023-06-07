#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include <llama-cpp/server/AppComponent.hpp>

#include <llama-cpp/server/controller/PromptController.hpp>
#include <llama-cpp/server/controller/StaticController.hpp>
#include <llama-cpp/server/controller/SwaggerApiController.hpp>

#include <oatpp-swagger/Controller.hpp>
#include <oatpp/network/Server.hpp>

std::string get_env_default(const std::string &env_name,
                            const std::string &defualt_value) {
  auto value = getenv(env_name.c_str());
  auto final = value == nullptr ? defualt_value : std::string(value);
  return final;
}

template <typename T>
std::shared_ptr<oatpp::web::server::api::ApiController>
add_endpoints(oatpp::web::server::api::Endpoints &docEndpoints,
              std::shared_ptr<T> controller) {
  static_assert(
      std::is_base_of<oatpp::web::server::api::ApiController, T>::value,
      "type parameter of this class must derive from ApiController.");
  if (is_swagger_enabled<T>) {
    docEndpoints.append(controller->getEndpoints());
  }
  return controller;
}

template <class... Args>
std::shared_ptr<oatpp::web::server::HttpRouter>
add_controllers(std::shared_ptr<oatpp::web::server::HttpRouter> router,
                Args... args) {
  using controller_t = std::shared_ptr<oatpp::web::server::api::ApiController>;

  oatpp::web::server::api::Endpoints docEndpoints;

  std::vector<controller_t> controllers = {
      add_endpoints(docEndpoints, args)...};

  router->addController(oatpp::swagger::Controller::createShared(docEndpoints));

  for (auto controller : controllers) {
    router->addController(controller);
  }

  return router;
}

void run() {
  register_signal_handler();

  AppComponent components; // Create scope Environment components

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  add_controllers(router, PromptController::createShared(),
                  StaticController::createShared());

  router->logRouterMappings();

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
