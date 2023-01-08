
function(grapphs_check_should_run_conan_install OUTPUT)
    if(DEFINED CONAN_EXPORTED)
        # We are being invoked from conan, nothing to do.
        set(${OUTPUT} FALSE PARENT_SCOPE)
        return()
    endif()

    if(GRAPPHS_COMPILE_TESTS OR GRAPPHS_COMPILE_SAMPLES)
        set(${OUTPUT} TRUE PARENT_SCOPE)
    else()
        set(${OUTPUT} FALSE PARENT_SCOPE)
    endif()
endfunction()

function(grapphs_try_run_conan_install)
    grapphs_check_should_run_conan_install(SHOULD_RUN)

    if(NOT SHOULD_RUN)
        message(STATUS "Skipping conan install")
        return()
    endif()

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(
                DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/release/0.17/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON
        )
    endif()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

    conan_cmake_configure(
            REQUIRES
            gtest/1.11.0
            nlohmann_json/3.10.5
            readosm/1.1.0a
            GENERATORS cmake
    )
    message(VERBOSE "Conan Settings: ${CONAN_SETTINGS}")
    if("${CONAN_SETTINGS}" STREQUAL "")
        conan_cmake_autodetect(CONAN_SETTINGS BUILD_TYPE ${CMAKE_BUILD_TYPE})
    endif()
    conan_cmake_install(
            PATH_OR_REFERENCE .
            BUILD missing
            SETTINGS ${CONAN_SETTINGS}
    )
    include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
endfunction()