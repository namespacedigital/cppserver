add_subdirectory(basic)

if (CPPSERVER_FEATURE_TLS)
    add_subdirectory(ssl)
endif()
