function(namespace.digital.cppserver.example target)
    target_link_libraries(${target} PRIVATE namespace.digital.server)
    set_target_properties(${TARGET} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${MALLOY_BINARY_DIR})
endfunction()
