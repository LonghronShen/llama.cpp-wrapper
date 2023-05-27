#ifndef BEEF6E96_10AE_4FDC_A563_C4E33AD652AC
#define BEEF6E96_10AE_4FDC_A563_C4E33AD652AC

#include <memory>
#include <string>
#include <vector>

#include <llama-cpp/models.hpp>
#include <llama-cpp/wrapper.h>

namespace llama_cpp {

class context {
public:
  static const std::int32_t max_token_count = 9999999;

public:
  context(const std::string &model_path, const model_options &options);
  context(const std::string &model_path, model_options::setters_t setters);
  ~context();

  void load();
  void unload();

  std::vector<float> token_embeddings(const std::vector<std::uint32_t> &tokens,
                                      const predict_options &options);

  std::vector<float> token_embeddings(const std::vector<std::uint32_t> &tokens,
                                      predict_options::setters_t setters);

  std::vector<float> embeddings(const std::string &text);

  std::vector<float> embeddings(const std::string &text,
                                const predict_options &options);

  std::vector<float> embeddings(const std::string &text,
                                predict_options::setters_t setters);

  std::string predict(const std::string &text);

  std::string predict(const std::string &text, const predict_options &options);

  std::string predict(const std::string &text,
                      predict_options::setters_t setters);

  void set_token_callback(const token_callback_t &callback);

  void set_predict_options(const predict_options &options);

  void set_predict_options(predict_options::setters_t setters);

private:
  void *_context;
  bool _embeddings;
  bool _initialized;
  std::string _model_path;
  std::int32_t _context_size;
  model_options _model_options;
  predict_options _predict_options;

  token_callback_t _callback;
  std::mutex _field_lock;
};

} // namespace llama_cpp

#endif /* BEEF6E96_10AE_4FDC_A563_C4E33AD652AC */
