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
  DTO_FIELD(Boolean, ignore_eos, "ignore_eos");
  DTO_FIELD(Vector<String>, stop_prompts, "stop_prompts");

  DTO_FIELD(Int32, n_predict, "max_tokens");
  DTO_FIELD(Int32, n_ctx, "n_ctx");
  DTO_FIELD(Int32, n_keep, "n_keep");
  DTO_FIELD(Int32, repeat_last_n, "repeat_last_n");
  DTO_FIELD(Int32, batch_size, "batch_size");
  DTO_FIELD(Int32, threads, "threads");

  DTO_FIELD(Float64, temperature, "temperature");
  DTO_FIELD(Float64, top_k, "top_k");
  DTO_FIELD(Float64, top_p, "top_p");
  DTO_FIELD(Float64, repeat_penalty, "repeat_penalty");
  DTO_FIELD(Float64, frequency_penalty, "frequency_penalty");
  DTO_FIELD(Float64, presence_penalty, "presence_penalty");
};

#include OATPP_CODEGEN_END(DTO)

#endif /* BDE41C87_385A_417C_8270_C45773F62DE8 */
