function(add_test_executable)
    set(prefix ARG)
    set(singleValues TEST_NAME)
    set(multiValues INCLUDES SOURCES LIBRARIES)

    cmake_parse_arguments("${prefix}" "" "${singleValues}" "${multiValues}" ${ARGN})

    set(TARGET_NAME ${ARG_TEST_NAME}_test)

    add_executable(${TARGET_NAME} ${ARG_SOURCES})

    add_test(NAME "${ARG_TEST_NAME}.t" COMMAND ${TARGET_NAME})

    add_dependencies(test ${TARGET_NAME})

    if (ARG_INCLUDES)
      execute_process (COMMAND bash -c "cd ../../deps/third_party && pwd" OUTPUT_VARIABLE pwd)
      execute_process (COMMAND bash -c "ls" OUTPUT_VARIABLE ls)
      message(STATUS "where I am      : " ${pwd})
   
      target_include_directories(${TARGET_NAME} PRIVATE ${ARG_INCLUDES})
    endif()

    if (ARG_LIBRARIES)
        target_link_libraries(${TARGET_NAME} PRIVATE ${ARG_LIBRARIES})
    endif()

    set_target_properties(${TARGET_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/t
        OUTPUT_NAME "${ARG_TEST_NAME}.t"
    )

endfunction()