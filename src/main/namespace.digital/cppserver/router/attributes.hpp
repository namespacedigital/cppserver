#ifndef NAMESPACE_DIGITAL_ATTRIBUTES_HPP
#define NAMESPACE_DIGITAL_ATTRIBUTES_HPP
#include <unordered_map>
#include "attribute.hpp"

namespace namespacedigital {
  namespace cppserver {
    namespace router {
      class Attributes {
      public:
        using attribute_storage = std::unordered_map<std::string, Attribute>;

        Attributes() = default;
        explicit Attributes(const std::string& str, char sep = '&');

        void insert(std::string key, std::string value);

        attribute_storage::const_iterator find(const std::string& key) const {
          return _attributes.find(key);
        }

        attribute_storage::const_iterator begin() const { return _attributes.begin(); }
        attribute_storage::const_iterator end() const { return _attributes.end(); }

        const Attribute& operator[](const std::string& key) const;

      private:
        attribute_storage _attributes;

        inline static const Attribute EMPTY;
      };


    }
  }
}
#endif