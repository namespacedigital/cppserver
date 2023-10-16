
# add components
add_subdirectory(configuration)
add_subdirectory(router)

# add server class
target_sources(
    ${TARGET}
    PUBLIC
        FILE_SET
            HEADERS
            BASE_DIRS ${CPPSERVER_BASE_DIR}
            FILES
                server.hpp

    PRIVATE
        server.cpp
)