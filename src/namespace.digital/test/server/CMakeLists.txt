include(../CMakeHelper.cmake)

add_test_executable(
    TEST_NAME server
    SOURCES
        server_test.cpp
    INCLUDES
        ../../vendor/third_party
    LIBRARIES
        namespace.digital-cppserver
)
