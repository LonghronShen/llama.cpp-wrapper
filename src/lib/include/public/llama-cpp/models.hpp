#ifndef C2CEDECD_8503_4CC3_AF7B_136D3478606B
#define C2CEDECD_8503_4CC3_AF7B_136D3478606B

#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <llama-cpp/utils.hpp>
#include <llama-cpp/wrapper.h>

namespace llama_cpp {

using token_callback_t = std::function<bool(void *, const std::string &)>;

template <typename T> using generic_setter_t = std::function<T && (T &)>;

struct model_options {
public:
  using setter_t = generic_setter_t<model_options>;
  using setters_t = std::initializer_list<setter_t>;

public:
  std::int32_t context_size = 512;
  std::int32_t seed = 0;
  std::int32_t parts = 0;
  bool f16_memory = false;
  bool embeddings = false;
  bool model_lock = false;

public:
  inline model_options() {}

  inline model_options(setters_t setters) {
    for (const auto &t : setters) {
      t(*this);
    }
  }

  inline model_options(const model_options &other)
      : context_size(other.context_size), seed(other.seed), parts(other.parts),
        f16_memory(other.f16_memory), embeddings(other.embeddings),
        model_lock(other.model_lock) {}
};

struct predict_options {
public:
  using setter_t = generic_setter_t<predict_options>;
  using setters_t = std::initializer_list<setter_t>;

public:
  std::int32_t seed = -1;
  std::int32_t threads =
      (std::int32_t)internal::threading::get_processor_count();
  std::int32_t tokens = 128;
  std::int32_t top_k;
  std::int32_t repeat = 64;
  std::int32_t batch = 8;
  std::int32_t n_keep = 64;
  std::int32_t n_ctx = 512;
  std::int32_t mirostat = 0;
  double top_p = 0.95;
  double temperature;
  double penalty = 1.1;
  double tail_free_sampling_z = 1.0;
  double typical_p = 1.0;
  double frequency_penalty = 0.0;
  double presence_penalty = 0.0;
  double mirostat_eta = 0.1;
  double mirostat_tau = 5.0;
  bool penalize_nl;
  bool f16kv;
  bool debug_mode;
  bool ignore_eos;
  std::vector<std::string> stop_prompts;
  std::string logit_bias;
  std::string prompts;
  token_callback_t token_callback;

public:
  inline predict_options() {}

  inline predict_options(setters_t setters) {
    for (const auto &t : setters) {
      t(*this);
    }
  }

  inline predict_options(const predict_options &other, setters_t setters)
      : predict_options(other) {
    for (const auto &t : setters) {
      t(*this);
    }
  }

  inline predict_options(const predict_options &other)
      : seed(other.seed), threads(other.threads), tokens(other.tokens),
        n_ctx(other.n_ctx), top_k(other.top_k), repeat(other.repeat),
        batch(other.batch), n_keep(other.n_keep), mirostat(other.mirostat),
        top_p(other.top_p), temperature(other.temperature),
        penalty(other.penalty),
        tail_free_sampling_z(other.tail_free_sampling_z),
        typical_p(other.typical_p), frequency_penalty(other.frequency_penalty),
        presence_penalty(other.presence_penalty),
        mirostat_eta(other.mirostat_eta), mirostat_tau(other.mirostat_tau),
        penalize_nl(other.penalize_nl), f16kv(other.f16kv),
        debug_mode(other.debug_mode), ignore_eos(other.ignore_eos),
        stop_prompts(other.stop_prompts), logit_bias(other.logit_bias),
        prompts(other.prompts), token_callback(other.token_callback) {}

  inline LLAMA_PARAMS to_llma_primitive_params() const {
    LLAMA_PARAMS params;

    params.ignore_eos = this->ignore_eos ? 1 : 0;
    params.memory_f16 = this->f16kv ? 1 : 0;
    params.penalize_nl = this->penalize_nl ? 1 : 0;
    params.frequency_penalty = this->frequency_penalty;
    params.mirostat_eta = this->mirostat_eta;
    params.mirostat_tau = this->mirostat_tau;
    params.presence_penalty = this->presence_penalty;
    params.repeat_penalty = this->penalty;
    params.temp = this->temperature;
    params.tfs_z = this->tail_free_sampling_z;
    params.top_p = this->top_p;
    params.typical_p = this->typical_p;
    params.mirostat = this->mirostat;
    params.n_batch = this->batch;
    params.n_keep = this->n_keep;
    params.n_ctx = this->n_ctx;
    params.repeat_last_n = this->repeat;
    params.seed = this->seed;
    params.threads = this->threads;
    params.tokens = this->tokens;
    params.top_k = this->top_k;

    params.antiprompt = nullptr;
    params.antiprompt_count = 0;

    params.logit_bias = this->logit_bias.c_str();
    params.prompt = "";

    return params;
  }

  inline void *to_llama_params() const {
    return this->to_llama_params(this->prompts);
  }

  inline void *to_llama_params(const std::string &prompts) const {
    const auto &options = this->to_llma_primitive_params();
    return llama_allocate_params_with(options, prompts, this->stop_prompts,
                                      this->logit_bias);
  }
};

} // namespace llama_cpp

#endif /* C2CEDECD_8503_4CC3_AF7B_136D3478606B */
