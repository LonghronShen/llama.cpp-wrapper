#ifndef BDE41C87_385A_417C_8270_C45773F62DE8
#define BDE41C87_385A_417C_8270_C45773F62DE8

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class PromptDto : public oatpp::DTO {
  DTO_INIT(PromptDto, DTO)

  DTO_FIELD(String, prompt, "prompt");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* BDE41C87_385A_417C_8270_C45773F62DE8 */
