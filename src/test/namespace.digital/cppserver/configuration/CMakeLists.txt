include(../../../CMakeHelper.cmake)

add_test_executable(
    TEST_NAME ini
    SOURCES
        driver/ini_test.cpp
    INCLUDES
        ../../deps/third_party
    LIBRARIES
        cppserver
)
