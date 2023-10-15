add_subdirectory(cppserver)

# Add tests (if supposed to)
if (CPPSERVER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()