# Define directory base path for FILE_SET
set(CPPSERVER_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Define target name
set(TARGET "namespace.digital-cppserver")

# Create library
add_library(${TARGET} STATIC)
add_library(namespace.digital::cppserver ALIAS namespace.digital-cppserver)

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

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/cppserver-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cppserver-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/cppserver
)

set(SPDLOG_INSTALL ON)

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
