#ifndef NAMESPACE_DIGITAL_HEADER_HPP
#define NAMESPACE_DIGITAL_HEADER_HPP
#include <string>

struct Header {
  std::string name;
  std::string value;
};



namespace namespacedigital {
  namespace cppserver {
    namespace router {
      namespace header {
        struct content_type {
          explicit content_type(const char* v) : value(v) {}
          std::string value;

          std::string operator()(const std::string&) const { return value; }
        };

        inline static const auto text_plain{ "text/plain" };
        inline static const auto text_html{ "text/html" };
        inline static const auto application_json{ "application/json" };
        inline static const auto image_x_icon{ "image/x-icon" };
        inline static const auto image_png{ "image/png" };
      }
    }
  }
}


// --------------------------------------------------------------------------

#endif /* header_hpp */