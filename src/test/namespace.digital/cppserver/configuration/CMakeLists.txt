include(../../../CMakeHelper.cmake)

add_test_executable(
    TEST_NAME server
    SOURCES
        driver/ini_test.cpp
    INCLUDES
        ../../vendor/third_party
    LIBRARIES
        namespace.digital-cppserver
)
