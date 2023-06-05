#ifndef B4041534_9105_4F61_B5A7_6C308A307B93
#define B4041534_9105_4F61_B5A7_6C308A307B93

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <functional>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <thread>

#if HAS_EXPERIMENTAL_ITERATOR_H
#include <experimental/iterator>
#else
#include <llama-cpp/compat/ostream_joiner>
#endif

#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
#include <boost/iterator/transform_iterator.hpp>
#else
#include <llama-cpp/compat/transform_iterator>
#endif

namespace llama_cpp {
namespace internal {
namespace stringutils {

bool inline starts_with(const std::string &input, const std::string &starting) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  return boost::algorithm::starts_with(input, starting);
#else
  return input.rfind(starting, 0) == 0;
#endif
}

bool inline ends_with(const std::string &input, const std::string &ending) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  return boost::algorithm::ends_with(input, ending);
#else
  if (input.length() >= ending.length()) {
    return (0 == input.compare(input.length() - ending.length(),
                               ending.length(), ending));
  } else {
    return false;
  }
#endif
}

std::string inline trim_start(const std::string &str) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  return boost::algorithm::trim_left_copy(str);
#else
  std::string s = str;
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
  return s;
#endif
}

std::string inline trim_start(const std::string &str,
                              const std::string &prefix) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  return boost::algorithm::replace_first_copy(str, prefix, "");
#else
  std::string s = str;
  s.replace(0, 0, "");
  return s;
#endif
}

std::string inline trim_start_if(const std::string &str,
                                 std::function<bool(char)> predict) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  return boost::algorithm::trim_left_copy_if(
      str, [&](char c) { return predict(c); });
#else
  std::string s = str;
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](unsigned char ch) {
            return !predict((char)ch);
          }));
  return s;
#endif
}

std::string inline trim_end(const std::string &str) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  return boost::algorithm::trim_right_copy(str);
#else
  std::string s = str;
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](unsigned char ch) { return !std::isspace(ch); })
              .base(),
          s.end());
  return s;
#endif
}

std::string inline trim_end(const std::string &str, const std::string &suffix) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  if (ends_with(str, suffix)) {
    return boost::algorithm::replace_tail_copy(str, suffix.length(), "");
  }
  return str;
#else
  if (ends_with(str, suffix)) {
    std::string s = str;
    s.erase(s.begin() + (s.length() - suffix.length()), s.end());
    return s;
  }
  return str;
#endif
}

std::string inline trim_end_if(const std::string &str,
                               std::function<bool(char)> predict) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  return boost::algorithm::trim_right_copy_if(
      str, [&](char c) { return predict(c); });
#else
  std::string s = str;
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [&](unsigned char ch) { return !predict(ch); })
              .base(),
          s.end());
  return s;
#endif
}

std::string inline trim(const std::string &str) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  return boost::algorithm::trim_copy(str);
#else
  return trim_end(trim_start(str));
#endif
}

std::string inline to_lower_copy(const std::string &input) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  return boost::algorithm::to_lower_copy(input);
#else
  std::string str_copy = input;
  std::transform(str_copy.begin(), str_copy.end(), str_copy.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return str_copy;
#endif
}

bool inline iequals(const std::string &a, const std::string &b) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  return boost::algorithm::iequals(a, b);
#else
  std::size_t sz = a.size();
  if (b.size() != sz) {
    return false;
  }
  for (std::size_t i = 0; i < sz; ++i) {
    if (std::tolower(a[i]) != std::tolower(b[i])) {
      return false;
    }
  }
  return true;
#endif
}

template <typename T> std::string to_string(const T &item) {
  std::stringstream ss;
  ss << item;
  ss.flush();
  return ss.str();
}

template <typename T, typename Transformer>
void print_vector(std::ostream &os, const std::vector<T> &v,
                  Transformer converter, const std::string &delimiter = ", ") {
  if (v.size()) {
    using element_type = typename std::vector<T>::value_type;

#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
    using namespace boost;
#else
    using namespace llama_cpp::compat;
#endif

    auto t_begin = make_transform_iterator(v.begin(), converter);
    auto t_end = make_transform_iterator(v.end(), converter);

    std::copy(
        t_begin, t_end,
#if HAS_EXPERIMENTAL_ITERATOR_H
        std::experimental::make_ostream_joiner(std::cout, delimiter.c_str()));
#else
        llama_cpp::compat::make_ostream_joiner<std::string>(std::cout,
                                                            delimiter.c_str()));
#endif
  }
}

template <typename T>
void print_vector(std::ostream &os, const std::vector<T> &v,
                  const std::string &delimiter = ", ") {
  using element_type = typename std::vector<T>::value_type;
  auto converter = [](const element_type &e) { return to_string(e); };
  print_vector(os, v, converter, delimiter);
}

inline void replace_all(std::string &str, const std::string &from,
                        const std::string &to) {
#if LLAMA_CPP_WRAPPER_USE_BOOST
  boost::algorithm::replace_all(str, from, to);
#else
  std::size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
#endif
}

inline std::string replace_all_copy(const std::string &s,
                                    const std::string &from,
                                    const std::string &to) {
#if LLAMA_CPP_WRAPPER_USE_BOOST
  return boost::algorithm::replace_all_copy(s, from, to);
#else
  std::string str = s;
  replace_all(str, from, to);
  return str;
#endif
}

std::vector<std::string> create_vector(const std::vector<std::string> &strings);

std::vector<std::string> create_vector(const char **strings, size_t length);

std::vector<std::string> create_vector(const char **strings, size_t count);

} // namespace stringutils

namespace threading {

std::size_t inline get_processor_count() {
  const auto processor_count = std::thread::hardware_concurrency();
  if (processor_count) {
    return processor_count;
  }
  return 1;
}

} // namespace threading

namespace filesystem {

std::string inline get_extension_from_path(const std::string &path) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  return boost::filesystem::extension(path);
#else
  std::filesystem::path filePath = path;
  return filePath.extension().generic_u8string();
#endif
}

std::string inline normalize_path(const std::string &messyPath) {
#ifdef LLAMA_CPP_WRAPPER_USE_BOOST
  const auto &path = boost::filesystem::canonical(messyPath);
  return path.generic_string();
#else
  std::filesystem::path path(messyPath);
  std::filesystem::path canonicalPath = std::filesystem::weakly_canonical(path);
  std::string npath = canonicalPath.make_preferred().string();
  return npath;
#endif
}

} // namespace filesystem
} // namespace internal
} // namespace llama_cpp

#endif /* B4041534_9105_4F61_B5A7_6C308A307B93 */
