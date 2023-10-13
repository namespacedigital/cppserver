# Define directory base path for FILE_SET
set(CPPSERVER_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Define target name
set(TARGET "namespace.digital-cppserver")

# Create library
add_library(${TARGET} STATIC)

target_link_libraries(${TARGET}
PUBLIC
  Boost::filesystem
  Boost::json
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

































# set_target_properties(
#   ${TARGET} 
#   PROPERTIES
#       CXX_STANDARD 20 
#       C_STANDARD 99
#       VERSION ${PROJECT_VERSION}
#       RUNTIME_OUTPUT_DIRECTORY ${CPPSERVER_BINARY_DIR} 
#       LIBRARY_OUTPUT_DIRECTORY ${CPPSERVER_BINARY_DIR}
# )


# Cppserver lib
add_subdirectory(namespace.digital)
