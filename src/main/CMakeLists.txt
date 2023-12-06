
# Define directory base path for FILE_SET
set(CPPSERVER_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Define target name
set(TARGET "cppserver")

# Define directory base path for Boost
set(BOOST_ROOT_DIR "" CACHE PATH "Boost installation directory\nIf it is an empty string, use the default search path of Boost")

# Build for different platforms boost can be 
if(CPPSERVER_PACKAGE_NAME STREQUAL "macos_arm64")
  set(BOOST_ROOT_DIR ${_INSTALL_DIR}/boost)
elseif(CPPSERVER_PACKAGE_NAME STREQUAL "raspberry-pi-os_armv7")
  set(BOOST_ROOT_DIR /root/boost)
  set(Boost_ARCHITECTURE 32)
elseif(CPPSERVER_PACKAGE_NAME STREQUAL "raspberry-pi-os_armv8")
  set(BOOST_ROOT_DIR /root/boost)
else()
endif()


if (NOT "${BOOST_ROOT_DIR}" STREQUAL "")
  list(APPEND CMAKE_PREFIX_PATH ${BOOST_ROOT_DIR})
endif()


set(Boost_USE_STATIC_LIBS ON)
if ("${TARGET_OS}" STREQUAL "windows")
  set(Boost_USE_STATIC_RUNTIME ON)
else()
  set(Boost_USE_STATIC_RUNTIME OFF)
endif()

# Create library
add_library(${TARGET} STATIC)

find_package(Boost REQUIRED COMPONENTS filesystem json)

target_link_libraries(${TARGET}
PUBLIC
  Boost::filesystem
  Boost::json
  spdlog::spdlog
  fmt::fmt
)

set_target_properties(
  ${TARGET} 
  PROPERTIES
      CXX_STANDARD 20 
      C_STANDARD 99
      VERSION ${PROJECT_VERSION}
      RUNTIME_OUTPUT_DIRECTORY ${CPPSERVER_BINARY_DIR} 
      LIBRARY_OUTPUT_DIRECTORY ${CPPSERVER_BINARY_DIR}
)

if (TARGET_OS STREQUAL "linux")
find_package(Threads REQUIRED)
target_link_libraries(${TARGET} 
PRIVATE
  Threads::Threads
)
endif()

if (TARGET_ARCH STREQUAL "arm")
if (USE_LIBCXX)
  target_include_directories(${TARGET}  PRIVATE ${SYSROOT}/usr/include/${ARCH_NAME})
  target_link_directories(${TARGET}  PRIVATE ${SYSROOT}/usr/lib/${ARCH_NAME})
endif()
endif()

add_subdirectory(namespace.digital)

########################################################################################################################
# Install                                                                                                              #
########################################################################################################################

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    cppserver-config-version.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_file(cppserver-config.cmake.in cppserver-config.cmake @ONLY)

configure_file(
    cppserver.pc.in
    cppserver.pc
    @ONLY
)

install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/cppserver.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/cppserver-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cppserver-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppserver
)

install(TARGETS ${TARGET} 
        EXPORT cppserver-targets
        FILE_SET HEADERS
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )
install(
    EXPORT cppserver-targets
    FILE cppserver-targets.cmake
    NAMESPACE namespace.digital::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppserver
)

