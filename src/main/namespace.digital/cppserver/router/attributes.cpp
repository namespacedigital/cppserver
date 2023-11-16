#include "attributes.hpp"
#include <regex>


using namespace namespacedigital::cppserver::router;

template<typename T>std::vector<std::string_view> split(const T& str, char sep) {
  std::vector<std::string_view> columns;
  size_t begin = 0;
  while (begin < str.size()) {
    size_t end = str.find(sep, begin);
    if (end == T::npos)
      end = str.size();

    columns.push_back(std::string_view(&str[begin], end - begin));
    begin = end + 1;
  }
  if (str.empty() || *str.rbegin() == sep)
    columns.push_back(std::string_view(&str[0] + begin, 0));

  return columns;
}

inline unsigned char hexdigit_to_num(unsigned char c) {
  return (c < 'A' ? c - '0' : toupper(c) - 'A' + 10);
}

std::string unescape(const std::string& s) {
  static std::regex escaped("%([0-9A-Fa-f]{2})");

  // Support urlencoded '+' --> ' '
  std::string t = std::regex_replace(s, std::regex("\\+"), " ");

  std::string unescaped;
  auto cur = std::sregex_token_iterator(t.begin(), t.end(), escaped);
  auto end = std::sregex_token_iterator();
  auto vbegin = t.cbegin();

  for (; cur != end; ++cur) {
    auto it = cur->first;

    // Append unescaped chars
    unescaped.append(vbegin, it);

    // Skip percent
    ++it;

    auto c = hexdigit_to_num(*it++) << 4;
    c |= hexdigit_to_num(*it++);

    unescaped += c;

    vbegin = cur->second;
  }

  unescaped.append(vbegin, t.cend());

  return unescaped;
}

Attributes::Attributes(const std::string& str, char sep) {
  for (auto&& a : split(str, sep)) {
    auto kv = split(a, '=');
    if (kv.size() == 2) {
      insert(std::string(kv[0]), std::string(kv[1]));
    }
  }
}

// --------------------------------------------------------------------------
void Attributes::insert(std::string key, std::string value) {
  _attributes.emplace(std::move(key), unescape(value));
}