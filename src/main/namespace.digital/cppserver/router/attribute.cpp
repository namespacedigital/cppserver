#include "attribute.hpp"
#include "attributes.hpp"

using namespace namespacedigital::cppserver::router;

const Attribute& Attributes::operator[](const std::string& key) const {
  auto found_key = _attributes.find(key);
  if (found_key != _attributes.end()) {
    return found_key->second;
  }

  return EMPTY;
}
