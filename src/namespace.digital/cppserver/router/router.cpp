#include "router.hpp"
#include <iostream>


using namespace namespacedigital::cppserver::router;

std::string Router::getHello() {
  std::string v = "hello world";
  routerLogger->error("on_connect(): {}", v);
  return v;
}