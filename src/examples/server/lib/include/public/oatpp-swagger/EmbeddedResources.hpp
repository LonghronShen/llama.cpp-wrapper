#ifndef DB88F8B5_F013_426B_A4B6_19CE42F7E9D4
#define DB88F8B5_F013_426B_A4B6_19CE42F7E9D4

#include <mime-types/mime_mapper.hpp>

#include <oatpp-swagger/Resources.hpp>

namespace oatpp {
namespace swagger {

class EmbeddedResources : public Resources {
public:
  EmbeddedResources();

  oatpp::String getResource(const oatpp::String &filename) override;

  std::string getMimeType(const std::string &filename) const override;
};

} // namespace swagger
} // namespace oatpp

#endif /* DB88F8B5_F013_426B_A4B6_19CE42F7E9D4 */
