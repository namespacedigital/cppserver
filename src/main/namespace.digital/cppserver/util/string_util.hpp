#ifndef NAMESPACE_DIGITAL_STRING_UTIL_HPP
#define NAMESPACE_DIGITAL_STRING_UTIL_HPP

namespace namespacedigital {
    namespace cppserver {
        namespace util {
            /**
             * Replace function
             */
            inline bool replace(std::string& str, const std::string& from, const std::string& to) {
                size_t start_pos = str.find(from);
                if (start_pos == std::string::npos)
                    return false;
                str.replace(start_pos, from.length(), to);
                return true;
            }

            inline std::vector<std::string>split(std::string& str, const std::string& delimiter) {
                std::vector<std::string> results;

                size_t pos = 0;
                std::string token;
                while ((pos = str.find(delimiter)) != std::string::npos) {
                    token = str.substr(0, pos);
                    // std::cout << token << std::endl;
                    results.push_back(token);
                    str.erase(0, pos + delimiter.length());
                }
                results.push_back(str);
                return results;
            }

        }
    }
}
#endif
