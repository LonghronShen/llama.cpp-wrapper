#include <algorithm>
#include <iterator>
#include <string>
#include <unordered_map>

#include <cmrc/cmrc.hpp>

#include <oatpp-swagger/Resources.hpp>

#include <oatpp-swagger/EmbeddedResources.hpp>

CMRC_DECLARE(swagger::ui::rc);

namespace oatpp {
namespace swagger {

EmbeddedResources::EmbeddedResources() : Resources(".", false) {}

oatpp::String EmbeddedResources::getResource(const oatpp::String &filename) {
  const auto &fs = cmrc::swagger::ui::rc::get_filesystem();
  if (fs.exists(filename)) {
    const auto &file = fs.open(filename);
    return std::string(file.begin(), file.end());
  }
  throw std::runtime_error(
      "[oatpp::swagger::Resources::getResource(...)]: Resource file not found. "
      "Please make sure: "
      "1. You are using correct version of oatpp-swagger. "
      "2. oatpp-swagger/res is not empty. "
      "3. You specified correct full path to oatpp-swagger/res folder");
}

std::string EmbeddedResources::getMimeType(const std::string &filename) const {
  return mime_types::get_type(filename);
}

} // namespace swagger
} // namespace oatpp
