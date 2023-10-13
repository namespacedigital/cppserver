#include "router.hpp"
#include <iostream>

using namespace namespacedigital::cppserver::router;

std::string Router::getHello() {
  std::string v = "hello world";
  return v;
}