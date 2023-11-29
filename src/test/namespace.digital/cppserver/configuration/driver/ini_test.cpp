#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../../../../../vendor/third_party/doctest/doctest.hpp"
#include <namespace.digital/cppserver/configuration/driver/ini.hpp>


TEST_CASE("Given wrong configuration file then should throw error") {
  auto& iniConfig = namespacedigital::cppserver::configuration::Ini::getInstance();
  CHECK_THROWS_WITH_AS(iniConfig.init("file.conf"), "file.conf: cannot open file", std::runtime_error);
}


TEST_CASE("Given correct configuration file should not throw error") {
  auto& iniConfig = namespacedigital::cppserver::configuration::Ini::getInstance();
  CHECK_NOTHROW(iniConfig.init("/Users/ovidiu.dragoi/my/cppserver/src/resource/test/test_config.ini"));
}

