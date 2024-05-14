include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)

#fmt
FetchContent_Declare(
    fmt 
    GIT_REPOSITORY  https://github.com/fmtlib/fmt
    GIT_TAG         10.1.1
)
FetchContent_GetProperties(fmt)
if (NOT fmt_POPULATED) 
    FetchContent_Populate(fmt)
    add_subdirectory(${fmt_SOURCE_DIR} ${fmt_BINARY_DIR})
endif()


# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG        v1.12.0
)
FetchContent_GetProperties(spdlog)
if (NOT spdlog_POPULATED) 
    FetchContent_Populate(spdlog)
    add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})

endif()
