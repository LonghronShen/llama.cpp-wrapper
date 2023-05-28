#ifndef B8D67753_5048_48D8_A674_02CD2DB8A37A
#define B8D67753_5048_48D8_A674_02CD2DB8A37A

#ifdef LLAMA_WRAPPER_SHARED
#if defined(_WIN32) && !defined(__MINGW32__)
#ifdef LLAMA_WRAPPER_BUILD
#define LLAMA_WRAPPER_API __declspec(dllexport)
#else
#define LLAMA_WRAPPER_API __declspec(dllimport)
#endif
#else
#define LLAMA_WRAPPER_API __attribute__((visibility("default")))
#endif
#else
#define LLAMA_WRAPPER_API
#endif

#ifdef __cplusplus
#include <functional>
#include <string>
#include <vector>

extern "C" {
#endif

#include <stdbool.h>

typedef unsigned char (*token_callback)(void *llama_context_ptr,
                                        char *token_str);

typedef struct {
  int ignore_eos;
  int memory_f16;
  int penalize_nl;
  const char **antiprompt;
  const char *logit_bias;
  const char *prompt;
  float frequency_penalty;
  float mirostat_eta;
  float mirostat_tau;
  float presence_penalty;
  float repeat_penalty;
  float temp;
  float tfs_z;
  float top_p;
  float typical_p;
  int antiprompt_count;
  int mirostat;
  int n_ctx;
  int n_batch;
  int n_keep;
  int repeat_last_n;
  int seed;
  int threads;
  int tokens;
  int top_k;
} * PLLAMA_PARAMS, LLAMA_PARAMS;

LLAMA_WRAPPER_API void register_signal_handler();

LLAMA_WRAPPER_API void set_token_callback(token_callback callback);

LLAMA_WRAPPER_API void *load_model(const char *fname, int n_ctx, int n_parts,
                                   int n_seed, bool memory_f16, bool mlock,
                                   bool embeddings);

LLAMA_WRAPPER_API int get_embeddings(void *params_ptr, void *state_pr,
                                     float *res_embeddings);

LLAMA_WRAPPER_API int get_token_embeddings(void *params_ptr, void *state_pr,
                                           int *tokens, int tokenSize,
                                           float *res_embeddings);

LLAMA_WRAPPER_API const char *llama_print_system_info(void);

LLAMA_WRAPPER_API void *llama_allocate_params(
    const char *prompt, int seed, int threads, int n_ctx, int tokens, int top_k,
    float top_p, float temp, float repeat_penalty, int repeat_last_n,
    int ignore_eos, int memory_f16, int n_batch, int n_keep,
    const char **antiprompt, int antiprompt_count, float tfs_z, float typical_p,
    float frequency_penalty, float presence_penalty, int mirostat,
    float mirostat_eta, float mirostat_tau, int penalize_nl,
    const char *logit_bias);

LLAMA_WRAPPER_API void *llama_allocate_params_ex(const LLAMA_PARAMS *params);

#ifdef __cplusplus
LLAMA_WRAPPER_API void *llama_allocate_params_with(
    const LLAMA_PARAMS &raw_params, const std::string &prompt,
    const std::vector<std::string> &antiprompts, const std::string &logit_bias);
#endif

LLAMA_WRAPPER_API void llama_free_params(void *params_ptr);

LLAMA_WRAPPER_API void llama_free_model(void *state);

LLAMA_WRAPPER_API int llama_predict(void *params_ptr, void *state_pr,
                                    char *result,
                                    std::uint64_t result_buffer_size,
                                    bool debug);

#ifdef __cplusplus
LLAMA_WRAPPER_API int llama_predict_ex(
    void *params_ptr, void *state_pr, char *result,
    std::uint64_t result_buffer_size, bool debug,
    std::function<bool(void *, const std::string &)> token_callback);
}
#endif

#endif /* B8D67753_5048_48D8_A674_02CD2DB8A37A */
