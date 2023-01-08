
find_package(Git REQUIRED)

function(grapphs_check_is_clean OUTPUT)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} status --short
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE STATUS_RESULT
            OUTPUT_VARIABLE STATUS_CONTENT
    )
    if(${STATUS_CONTENT} STREQUAL "")
        set(${OUTPUT} TRUE)
    else()
        set(${OUTPUT} FALSE)
    endif()
endfunction()

function(grapphs_detect_version VERSION_OUTPUT)

    set(GPP_VERSION "")
    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags --always --first-parent --long
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE TAG_RESULT
            OUTPUT_VARIABLE TAG_CONTENT
    )
    string(STRIP ${TAG_CONTENT} TAG_CONTENT)
    if("${TAG_CONTENT}" MATCHES "([a-zA-Z0-9_\\.]+)-([0-9]+)-g([a-zA-Z0-9]+)")
        set(GPP_VERSION_TAG "${CMAKE_MATCH_1}")
        set(GPP_NUM_COMMITS_AHEAD "${CMAKE_MATCH_2}")
        set(GPP_VERSION_REV "${CMAKE_MATCH_3}")
        string(APPEND GPP_VERSION "${GPP_VERSION_TAG}")
        if(GPP_NUM_COMMITS_AHEAD GREATER 0)
            string(APPEND GPP_VERSION "-${GPP_VERSION_REV}")
        endif()
        if("${GPP_VERSION_TAG}" MATCHES "v?([0-9]+).([0-9]+).([0-9]+)")
            set(${VERSION_OUTPUT}_MAJOR ${CMAKE_MATCH_1} PARENT_SCOPE)
            set(${VERSION_OUTPUT}_MINOR ${CMAKE_MATCH_2} PARENT_SCOPE)
            set(${VERSION_OUTPUT}_PATCH ${CMAKE_MATCH_3} PARENT_SCOPE)
            set(
                    ${VERSION_OUTPUT}_FULL
                    "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}"
                    PARENT_SCOPE
            )
        endif()
    else()
        string(APPEND GPP_VERSION ${TAG_CONTENT})
    endif()


    grapphs_check_is_clean(GIT_IS_CLEAN)
    if(NOT GIT_IS_CLEAN)
        string(APPEND GPP_VERSION "-dirty")
    endif()
    set(${VERSION_OUTPUT} ${GPP_VERSION} PARENT_SCOPE)
endfunction()