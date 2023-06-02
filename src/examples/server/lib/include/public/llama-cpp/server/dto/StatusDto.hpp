#ifndef CEE116D0_FCA1_47BE_93D9_06D5F3D55C0C
#define CEE116D0_FCA1_47BE_93D9_06D5F3D55C0C

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>


#include OATPP_CODEGEN_BEGIN(DTO)

class StatusDto : public oatpp::DTO {

  DTO_INIT(StatusDto, DTO)

  DTO_FIELD_INFO(status) { info->description = "Short status text"; }
  DTO_FIELD(String, status);

  DTO_FIELD_INFO(code) { info->description = "Status code"; }
  DTO_FIELD(Int32, code);

  DTO_FIELD_INFO(message) { info->description = "Verbose message"; }
  DTO_FIELD(String, message);
};

#include OATPP_CODEGEN_END(DTO)

#endif /* CEE116D0_FCA1_47BE_93D9_06D5F3D55C0C */