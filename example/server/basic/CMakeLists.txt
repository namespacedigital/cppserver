# Set a target name
set(TARGET basic_server)

# Create the executable
add_executable(${TARGET})

# Setup example
target_link_libraries(${TARGET} PRIVATE namespace.digital::cppserver)

set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CPPSERVER_BINARY_DIR})

# Add source files
target_sources(
    ${TARGET}
    PRIVATE
        main.cpp
)
