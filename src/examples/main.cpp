#include <cctype>
#include <chrono>
#include <csignal>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <type_traits>

#include <argh.h>
#include <isocline.h>

#include <llama-cpp/llama.hpp>
#include <llama-cpp/models.hpp>
#include <llama-cpp/utils.hpp>
#include <llama-cpp/wrapper.h>

using namespace std;
using namespace llama_cpp::internal::stringutils;

const std::uint32_t COLOR_RED = 0xFF0000;
const std::uint32_t COLOR_YELLOW = 0xFFFF00;

inline void write_prompt(const std::string &prompt,
                         std::uint32_t color = 0xFF0000) {
  ic_term_color_rgb(true, color);
  ic_term_write(prompt.c_str());
  ic_term_flush();
  ic_term_reset();
}

template <typename T>
const T read_args(const argh::parser &cmdl,
                  std::initializer_list<char const *const> pre_reg_names,
                  const T &default_value) {
  T value;
  cmdl(pre_reg_names, default_value) >> value;
  return value;
}

const bool read_args(const argh::parser &cmdl,
                     std::initializer_list<char const *const> pre_reg_names,
                     const bool &default_value) {
  std::string value;
  cmdl(pre_reg_names, default_value ? "true" : "false") >> value;
  return iequals(value, "true");
}

inline std::string load_prompts(const std::string &prompts_file_path,
                                const std::string &user_name,
                                const std::string &ai_name,
                                std::size_t max_length) {
  try {
    if (prompts_file_path.empty()) {
      return "";
    }

    std::ifstream file(prompts_file_path);

    if (!file) {
      fprintf(stdout, "error: failed to open file '%s'\n",
              prompts_file_path.c_str());
      return "";
    }

    std::string prompt;
    std::copy(std::istreambuf_iterator<char>(file),
              std::istreambuf_iterator<char>(), back_inserter(prompt));

    llama_cpp::internal::stringutils::replace_all(prompt, "[[USER_NAME]]",
                                                  user_name);
    llama_cpp::internal::stringutils::replace_all(prompt, "[[AI_NAME]]",
                                                  ai_name);

    if (prompt.length() > max_length) {
      prompt.erase(max_length, std::string::npos);
    }

    if (prompt.back() == '\n') {
      prompt.pop_back();
    }

    return prompt;
  } catch (...) {
    return "";
  }
}

int main(int argc, char *argv[]) {
  register_signal_handler();
  atexit([]() {
    ic_term_reset();
    ic_term_done();
  });

  ic_term_init();
  ic_set_prompt_marker(": ", "> ");

  const auto processor_count =
      llama_cpp::internal::threading::get_processor_count();

  argh::parser cmdl;
  cmdl.parse(argc, argv, argh::parser::PREFER_PARAM_FOR_UNREG_OPTION);

  const auto &model = read_args<std::string>(cmdl, {"-m", "--model"},
                                             "./models/7B/ggml-model-q4_0.bin");
  const auto &prompt_file =
      read_args<std::string>(cmdl, {"-f", "--prompt-file"}, "");

  const auto &threads = read_args(cmdl, {"-t", "--threads"}, processor_count);
  const auto &n_predict = read_args(cmdl, {"-n", "--n_predict"}, -1);
  const auto &repeat_penalty = read_args(cmdl, {"--repeat_penalty"}, 1.1f);
  const auto &temperature = read_args(cmdl, {"--temp"}, 0.8f);
  const auto &top_k = read_args(cmdl, {"--top_k"}, 40);
  const auto &top_p = read_args(cmdl, {"--top_p"}, 0.95f);
  const auto &n_keep = read_args(cmdl, {"--n_keep"}, 0);
  const auto &repeat_last_n = read_args(cmdl, {"--repeat_last_n"}, 64);
  const auto &batch_size = read_args(cmdl, {"--batch_size"}, 512);
  const auto &ctx_size = read_args(cmdl, {"-c", "--ctx_size"}, 512);
  const auto &ignore_eos = read_args(cmdl, {"--ignore-eos"}, false);

  const auto &user_prompt =
      read_args<std::string>(cmdl, {"-r", "--reverse"}, "User");
  const auto &ai_prompt =
      read_args<std::string>(cmdl, {"-a", "--ai-name"}, "AI");
  const auto &debugMode = read_args(cmdl, {"-d"}, false);

  const auto &preload_prompts =
      load_prompts(prompt_file, user_prompt, ai_prompt, ctx_size);

  std::vector<std::string> stop_prompts = {user_prompt + ": "};

  try {
    if (debugMode) {
      fprintf(stdout, "system_info: n_threads = %lu / %d | %s\n", threads,
              std::thread::hardware_concurrency(), llama_print_system_info());
    }

    llama_cpp::context ctx(model, {[&](llama_cpp::model_options &opts) {
                             opts.context_size = ctx_size;
                             return opts;
                           }});

    ctx.load();
    ctx.set_predict_options({[&](llama_cpp::predict_options &po) {
      po.n_ctx = ctx_size;
      po.debug_mode = debugMode;
      po.ignore_eos = ignore_eos;
      po.penalty = repeat_penalty;
      po.tokens = n_predict;
      po.temperature = temperature;
      po.top_k = top_k;
      po.top_p = top_p;
      po.n_keep = n_keep;
      po.repeat = repeat_last_n;
      po.batch = batch_size;
      po.threads = (std::int32_t)threads;
      po.stop_prompts = stop_prompts;
      po.token_callback = [&](void *_ctx, const std::string &ch) {
        using namespace llama_cpp::internal::stringutils;

        try {
          auto piece = trim_start_if(
              ch, [](char c) { return c == '\n' || !std::isprint(c); });

          if (piece == "") {
            return true;
          }

          for (const auto &sw : stop_prompts) {
            if (sw.length() > 0 && sw.length() < piece.length()) {
              piece = trim_end(piece, sw);
            }
          }

          ic_term_write(piece.c_str());
          ic_term_flush();

          return true;
        } catch (const std::exception &ex) {
          std::cerr << "Error: " << ex.what() << std::endl;
          return false;
        }
      };
      return po;
    }});

    char *input =
        preload_prompts.empty() ? nullptr : (char *)preload_prompts.c_str();

    bool skip_prompts = false;
    if (input != nullptr) {
      skip_prompts = true;
      // ic_term_color_rgb(true, COLOR_YELLOW);
    }

    while (input != nullptr ||
           (input = ic_readline(user_prompt.c_str())) != nullptr) {
      std::string text = input;

      if (!skip_prompts) {
        ic_free(input);
      } else {
        skip_prompts = false;
      }

      write_prompt(ai_prompt + ": ", COLOR_RED);

      input = nullptr;

      const auto &final_aws = ctx.predict(text);

      // if (debugMode) {
      //   const auto &embeds = ctx.embeddings(text);
      //   std::cout << "Embeddings: ";
      //   print_vector(std::cout, embeds);
      //   std::cout << std::endl;
      // }

      std::cout << std::endl;
    }
  } catch (std::exception &ex) {
    std::cout << "error: " << ex.what() << std::endl;
    return 1;
  }

  return 0;
}