#include <iostream>
#include "route.hpp"

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


Route::Route(const std::string& path, route_cb&& cb) :
  _path(path),
  _cb(std::move(cb)) {
  if (path.empty() || path[0] != '/') {
    throw std::runtime_error("Route path [" + path + "] must begin with '/'.");
  }

  for (auto&& p : split(path, '/')) {
    _segments.emplace_back(p);
  }

}

// --------------------------------------------------------------------------
bool Route::match(Request& req) const noexcept {

  // Remove attributes and target split
  auto target_split = split(std::string_view{ req.target().data(), req.target().size() }, '?');
  auto request_paths = split(target_split[0], '/');

  if (_segments.size() != request_paths.size()) {
    return false;
  }

  std::string attrs = (target_split.size() > 1 ? std::string(target_split[1]) : "");

  for (std::size_t i = 0; i < _segments.size(); ++i) {
    auto& segment = _segments[i];
    auto& request_segment = request_paths[i];

    if (segment[0] == ':') {
      attrs += (attrs.empty() ? "" : "&")
        + std::string(&segment[1], segment.size() - 1)
        + "="
        + std::string(request_segment);
    } else if (segment != request_segment) {
      return false;
    }
  }

  if (!attrs.empty()) {
    req.get_attributes() = Attributes(attrs);
  }

  return true;
}
