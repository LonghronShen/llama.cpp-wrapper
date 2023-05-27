#include <llama-cpp/utils.hpp>

namespace llama_cpp {
namespace internal {
namespace stringutils {

std::vector<std::string>
create_vector(const std::vector<std::string> &strings) {
  std::vector<std::string> vec;
  for (size_t i = 0; i < strings.size(); i++) {
    vec.push_back(std::string(strings[i]));
  }
  return vec;
}

std::vector<std::string> create_vector(const char **strings, size_t length) {
  std::vector<std::string> vec;
  for (std::size_t i = 0; i < length; i++) {
    vec.push_back(std::string(strings[i]));
  }
  return vec;
}

} // namespace stringutils
} // namespace internal
} // namespace llama_cpp