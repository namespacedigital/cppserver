cmake_minimum_required(VERSION 3.23)

# Project
project(cppserver VERSION ${CPPSERVER_VERSION} LANGUAGES CXX HOMEPAGE_URL "https://github.com/namespacedigital/cppserver")

# Options
option(CPPSERVER_BUILD_EXAMPLES "Whether to build examples"       ON)
option(CPPSERVER_BUILD_TESTS    "Whether to build tests"          ON)
option(CPPSERVER_FEATURE_TLS    "Whether to enable TLS features"  OFF)
option(CPPSERVER_DEPENDENCY_SPDLOG_DOWNLOAD "Whether to automatically fetch spdlog" ON)
option(CPPSERVER_DEPENDENCY_FMT_DOWNLOAD "Whether to automatically fetch fmt" ON)

# Settings
set(CPPSERVER_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin) # Needed for later

# Parse and set parameters settings
set(CPPSERVER_PACKAGE_NAME "" CACHE STRING "Package name to automate build settings")
set(TARGET_OS "" CACHE STRING "Operating OS to build for: windows, macos, linux")
set(TARGET_OS_LINUX "" CACHE STRING "TARGET_OS linux")
set(TARGET_ARCH "" CACHE STRING "A working CPU to build for: x86_64, arm")
set(TARGET_ARCH_ARM "" CACHE STRING "Detailed CPU information when TARGET_ARCH is arm.Valid values ​​are armv6, armv7, armv8")
set(BOOST_ROOT_DIR "" CACHE PATH "Boost installation directory\nIf it is an empty string, use the default search path of Boost")
set(CLANG_ROOT "" CACHE PATH "Location of the clang compiler used for compilation")
set(USE_LIBCXX OFF CACHE BOOL "Whether to use libc++ instead of libstdc++")
set(LIBCXX_INCLUDE_DIR "" CACHE PATH "libc++ include directory when using libc++ .If it is an empty string, use libc++ in the default search path")
set(SYSROOT "" CACHE PATH "RootFS directory for cross-builds when TARGET_ARCH is arm")

# Set dependencies accordingly
set(CPPSERVER_DEPENDENCY_OPENSSL OFF)
if (CPPSERVER_FEATURE_TLS)
    set(CPPSERVER_DEPENDENCY_OPENSSL ON)
endif()


# Set for OS
if(CPPSERVER_PACKAGE_NAME STREQUAL "macos_arm64")
  message(CHECK_START "build for macos_arm64")  
  set(_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/build/macos_arm64/_install)
  set(_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/build/macos_arm64/_build)
  set(_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/third_party)
  set(TARGET_OS "macos")
  set(TARGET_ARCH "arm64")
  set(BOOST_ROOT_DIR ${_INSTALL_DIR}/boost)
elseif(CPPSERVER_PACKAGE_NAME STREQUAL "raspberry-pi-os_armv7")
  set(TARGET_OS "linux")
  set(TARGET_OS_LINUX "raspberry-pi-os")
  set(TARGET_ARCH "arm")
  set(TARGET_ARCH_ARM "armv7")
  set(BOOST_ROOT_DIR /root/boost)
  set(CLANG_ROOT /root/llvm/clang)
  set(SYSROOT /root/rootfs)
  set(USE_LIBCXX ON) 
  set(LIBCXX_INCLUDE_DIR /root/llvm/libcxx/include)
  set(Boost_ARCHITECTURE 32)
elseif(CPPSERVER_PACKAGE_NAME STREQUAL "raspberry-pi-os_armv8")
  set(TARGET_OS "linux")
  set(TARGET_OS_LINUX "raspberry-pi-os")
  set(TARGET_ARCH "arm")
  set(TARGET_ARCH_ARM "armv8")
  set(BOOST_ROOT_DIR /root/boost)
  set(CLANG_ROOT /root/llvm/clang)
  set(SYSROOT /root/rootfs)
  set(USE_LIBCXX ON)
  set(LIBCXX_INCLUDE_DIR /root/llvm/libcxx/include)
else()
endif()

# Add the source code library
add_subdirectory(src/namespace.digital)

add_subdirectory(example)