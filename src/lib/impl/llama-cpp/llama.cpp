#include <cstddef>
#include <exception>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

#include <llama-cpp/llama.hpp>
#include <llama-cpp/utils.hpp>
#include <llama-cpp/wrapper.h>

namespace llama_cpp {

context::context(const std::string &model_path, const model_options &options)
    : _model_path(model_path), _model_options(options), _initialized(false) {}

context::context(const std::string &model_path,
                 model_options::setters_t setters)
    : _model_path(model_path), _model_options(setters), _initialized(false) {}

context::~context() { this->unload(); }

void context::load() {
  std::lock_guard<std::mutex> field_lock(this->_field_lock);

  if (!this->_initialized) {
    const auto result = load_model(
        this->_model_path.c_str(), this->_model_options.context_size,
        this->_model_options.parts, this->_model_options.seed,
        this->_model_options.f16_memory, this->_model_options.model_lock,
        this->_model_options.embeddings);

    if (result == nullptr) {
      throw std::runtime_error("failed loading model");
    }

    this->_context = result;
    this->_initialized = true;
  }
}

void context::unload() {
  std::lock_guard<std::mutex> field_lock(this->_field_lock);
  if (this->_initialized) {
    if (this->_context != nullptr) {
      llama_free_model(this->_context);
      this->_embeddings = this->_model_options.embeddings;
      this->_context_size = this->_model_options.context_size;
      this->_initialized = false;
      this->_context = nullptr;
    }
  }
}

void context::set_predict_options(const predict_options &options) {
  std::lock_guard<std::mutex> field_lock(this->_field_lock);
  this->_predict_options = options;
}

void context::set_predict_options(predict_options::setters_t setters) {
  std::lock_guard<std::mutex> field_lock(this->_field_lock);
  this->_predict_options = predict_options(setters);
}

std::vector<float>
context::token_embeddings(const std::vector<std::uint32_t> &tokens,
                          predict_options::setters_t setters) {
  predict_options options(this->_predict_options, setters);
  return token_embeddings(tokens, options);
}

std::vector<float>
context::token_embeddings(const std::vector<std::uint32_t> &tokens,
                          const predict_options &options) {
  if (!this->_embeddings) {
    throw std::runtime_error("model loaded without embeddings");
  }

  auto outTokens = options.tokens;
  if (outTokens == 0) {
    outTokens = max_token_count;
  }

  std::vector<float> floats(outTokens, 0.0f);

  const auto params = options.to_llama_params();

  const auto result = get_token_embeddings(
      params, this->_context, (int *)tokens.data(), tokens.size(), &floats[0]);

  if (result != 0) {
    throw std::runtime_error("embedding inference failed");
  }

  return floats;
}

std::vector<float> context::embeddings(const std::string &text) {
  return embeddings(text, this->_predict_options);
}

std::vector<float> context::embeddings(const std::string &text,
                                       predict_options::setters_t setters) {
  predict_options options(this->_predict_options, setters);
  return embeddings(text, options);
}

std::vector<float> context::embeddings(const std::string &text,
                                       const predict_options &options) {
  if (!this->_embeddings) {
    throw std::runtime_error("model loaded without embeddings");
  }

  auto outTokens = options.tokens;
  if (outTokens == 0) {
    outTokens = max_token_count;
  }

  std::vector<float> floats(outTokens, 0.0f);

  const auto params = options.to_llama_params();

  const auto result = get_embeddings(params, this->_context, &floats[0]);

  if (result != 0) {
    throw std::runtime_error("embedding inference failed");
  }

  return floats;
}

std::string context::predict(const std::string &text) {
  return predict(text, this->_predict_options);
}

std::string context::predict(const std::string &text,
                             predict_options::setters_t setters) {
  predict_options options(this->_predict_options, setters);
  return predict(text, options);
}

std::string context::predict(const std::string &text,
                             const predict_options &options) {
  auto outTokens = options.tokens;
  if (outTokens == 0) {
    outTokens = max_token_count;
  }

  char *output_buffer = nullptr;
  if (options.tokens > 0) {
    output_buffer = new char[options.tokens];
    memset(output_buffer, 0, options.tokens);
  }

  const auto params = options.to_llama_params(text);
  const auto result =
      llama_predict_ex(params, this->_context, output_buffer,
                       options.tokens > 0 ? options.tokens : 0,
                       options.debug_mode, options.token_callback);
  if (result != 0) {
    throw std::runtime_error("inference failed");
  }

  llama_free_params(params);

  if (output_buffer != nullptr) {
    std::string out_str = output_buffer;
    delete[] output_buffer;

    out_str = internal::stringutils::trim_start_if(
        out_str, [](char c) { return !std::isspace(c) && c != '\n'; });

    if (internal::stringutils::starts_with(out_str, text)) {
      out_str = internal::stringutils::trim_start(out_str, text);
    }

    for (const auto &sw : options.stop_prompts) {
      out_str = internal::stringutils::trim_end(out_str, sw);
    }

    return out_str;
  }

  return "";
}

void context::set_token_callback(const token_callback_t &callback) {
  std::lock_guard<std::mutex> field_lock(this->_field_lock);
  this->_callback = callback;
}

} // namespace llama_cpp