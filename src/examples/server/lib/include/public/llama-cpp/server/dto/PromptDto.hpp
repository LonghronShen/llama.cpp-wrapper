#ifndef BDE41C87_385A_417C_8270_C45773F62DE8
#define BDE41C87_385A_417C_8270_C45773F62DE8

#include <oatpp/core/Types.hpp>
#include <oatpp/core/macro/codegen.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class PromptDto : public oatpp::DTO {
  DTO_INIT(PromptDto, DTO)

  DTO_FIELD(String, session_id, "session_id");

  DTO_FIELD(String, prompt, "prompt");
  DTO_FIELD(String, bot, "bot");

  DTO_FIELD(String, model, "model");
  DTO_FIELD(Float32, temperature, "temperature");
  DTO_FIELD(Int32, max_tokens, "max_tokens");
  DTO_FIELD(Float32, top_p, "top_p");
  DTO_FIELD(Float32, frequency_penalty, "frequency_penalty");
  DTO_FIELD(Float32, presence_penalty, "presence_penalty");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* BDE41C87_385A_417C_8270_C45773F62DE8 */
