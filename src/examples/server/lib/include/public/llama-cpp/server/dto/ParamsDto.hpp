#ifndef A2C03B08_789A_4F27_9323_358CFE6DFD27
#define A2C03B08_789A_4F27_9323_358CFE6DFD27

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class ParamsDto : public oatpp::DTO {
  DTO_INIT(ParamsDto, DTO)

  DTO_FIELD(Vector<String>, models, "models");
  DTO_FIELD(Int32, max_threads_count, "max_threads_count");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* A2C03B08_789A_4F27_9323_358CFE6DFD27 */
