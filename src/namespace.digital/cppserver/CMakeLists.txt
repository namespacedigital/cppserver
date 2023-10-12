# Define directory base path for FILE_SET
set(CPPSERVER_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Define target name
set(TARGET "server")

# Create library
add_library(${TARGET} STATIC)


set_target_properties(
  ${TARGET} 
  PROPERTIES
      VERSION ${PROJECT_VERSION}
      RUNTIME_OUTPUT_DIRECTORY ${CPPSERVER_BINARY_DIR} 
      LIBRARY_OUTPUT_DIRECTORY ${CPPSERVER_BINARY_DIR}
)

# Cppserver lib
add_subdirectory(configuration)
add_subdirectory(server)
