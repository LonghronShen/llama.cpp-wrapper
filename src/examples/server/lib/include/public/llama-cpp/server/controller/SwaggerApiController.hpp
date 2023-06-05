#ifndef D41EB7EF_E377_43E0_BF1B_8192AF53C80C
#define D41EB7EF_E377_43E0_BF1B_8192AF53C80C

#include <oatpp/web/server/api/ApiController.hpp>

class SwaggerApiController : public oatpp::web::server::api::ApiController {
public:
  SwaggerApiController(const std::shared_ptr<oatpp::data::mapping::ObjectMapper>
                           &defaultObjectMapper,
                       const oatpp::String &routerPrefix = nullptr)
      : oatpp::web::server::api::ApiController(defaultObjectMapper,
                                               routerPrefix) {}
};

template <typename... Ts>
constexpr std::true_type check_swagger_enabled(const SwaggerApiController &);
constexpr std::false_type check_swagger_enabled(...);

template <typename T>
constexpr bool is_swagger_enabled =
    decltype(check_swagger_enabled(std::declval<T>()))::value;

#endif /* D41EB7EF_E377_43E0_BF1B_8192AF53C80C */
