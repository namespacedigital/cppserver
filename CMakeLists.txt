cmake_minimum_required(VERSION 3.23)

# Only interpret if() arguments as variables or keywords when unquoted.
cmake_policy(SET CMP0054 NEW)
# MSVC runtime library flags are selected by an abstraction.
cmake_policy(SET CMP0091 NEW)
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


if(STREAM_PACKAGE_NAME STREQUAL "macos_arm64")
  message(CHECK_START "build for macos_arm64")  
  set(_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platform/macos_arm64/_install)
  set(_BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/platform/macos_arm64/_build)
  set(_THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
  set(TARGET_OS "macos")
  set(TARGET_ARCH "arm64")
  set(BOOST_ROOT_DIR ${_INSTALL_DIR}/boost)

elseif(MOMO_PACKAGE_NAME STREQUAL "raspberry-pi-os_armv7")

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

if (NOT "${CLANG_ROOT}" STREQUAL "")
  set(CMAKE_C_COMPILER ${CLANG_ROOT}/bin/clang)
  set(CMAKE_CXX_COMPILER ${CLANG_ROOT}/bin/clang++)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if (NOT "${BOOST_ROOT_DIR}" STREQUAL "")
  list(APPEND CMAKE_PREFIX_PATH ${BOOST_ROOT_DIR})
endif()


# SYSROOT this setting must be done before project
if (TARGET_OS STREQUAL "linux")
  if (TARGET_ARCH STREQUAL "arm")
    if (TARGET_ARCH_ARM STREQUAL "armv8")
      set(ARCH_NAME aarch64-linux-gnu)
    else()
      set(ARCH_NAME arm-linux-gnueabihf)
    endif()

    set(CMAKE_SYSTEM_NAME Linux)
    if (TARGET_ARCH_ARM STREQUAL "armv8")
      set(CMAKE_SYSTEM_PROCESSOR aarch64)
    else()
      set(CMAKE_SYSTEM_PROCESSOR arm)
    endif()
    set(CMAKE_SYSROOT ${SYSROOT})
    set(CMAKE_C_COMPILER_TARGET ${ARCH_NAME})
    set(CMAKE_CXX_COMPILER_TARGET ${ARCH_NAME})

    set(CMAKE_FIND_ROOT_PATH ${SYSROOT})
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
  endif()
endif()

if (TARGET_OS STREQUAL "macos")
  set(ARCH_NAME aarch64-apple-darwin)
  set(CMAKE_SYSTEM_PROCESSOR arm64)

  set(CMAKE_C_COMPILER_TARGET ${ARCH_NAME})
  set(CMAKE_CXX_COMPILER_TARGET ${ARCH_NAME})
endif()

project(namespace.digital VERSION ${CPPSERVER_VERSION} LANGUAGES C CXX HOMEPAGE_URL "https://github.com/namespacedigital/cppserver")

list(APPEND MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(Boost_USE_STATIC_LIBS ON)
if ("${TARGET_OS}" STREQUAL "windows")
  set(Boost_USE_STATIC_RUNTIME ON)
else()
  set(Boost_USE_STATIC_RUNTIME OFF)
endif()

# find_package(Boost REQUIRED COMPONENTS filesystem json)

# add_executable(namespace.digital)

# target_include_directories(namespace.digital PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# target_sources(namespace.digital
#   PRIVATE
#   main.cpp
# )

# target_include_directories(namespace.digital PRIVATE src)

# set_target_properties(namespace.digital PROPERTIES CXX_STANDARD 20 C_STANDARD 99)

# target_link_libraries(namespace.digital
#   PUBLIC
#     Boost::filesystem
#     Boost::json
# )


# if (TARGET_OS STREQUAL "linux")
#   find_package(Threads REQUIRED)
#   target_link_libraries(namespace.digital
#   PRIVATE
#     Threads::Threads
# )
# endif()

# if (TARGET_ARCH STREQUAL "arm")
#   if (USE_LIBCXX)
#     target_include_directories(namespace.digital PRIVATE ${SYSROOT}/usr/include/${ARCH_NAME})
#     target_link_directories(namespace.digital PRIVATE ${SYSROOT}/usr/lib/${ARCH_NAME})
#   endif()
# endif()
