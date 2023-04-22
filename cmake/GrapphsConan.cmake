
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

    get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
    if(is_multi_config)
        set(conan_generator cmake_find_package_multi)
    else()
        set(conan_generator cmake_find_package)
    endif()

    message(VERBOSE "Conan Settings: ${CONAN_SETTINGS}")

    if(GRAPPHS_COMPILE_SAMPLES)
        list(APPEND CONAN_OPTIONS "samples=True")
    else()
        list(APPEND CONAN_OPTIONS "samples=False")
    endif()

    if(GRAPPHS_COMPILE_TESTS)
        list(APPEND CONAN_OPTIONS "samples=True")
    else()
        list(APPEND CONAN_OPTIONS "samples=False")
    endif()

    if(GRAPPHS_COMPILE_SVG)
        list(APPEND CONAN_OPTIONS "samples=True")
    else()
        list(APPEND CONAN_OPTIONS "samples=False")
    endif()

    if(GRAPPHS_COMPILE_GRAPHVIZ)
        list(APPEND CONAN_OPTIONS "samples=True")
    else()
        list(APPEND CONAN_OPTIONS "samples=False")
    endif()

    if("${CONAN_SETTINGS}" STREQUAL "")
        if(is_multi_config)
            foreach(TYPE ${CMAKE_CONFIGURATION_TYPES})
                conan_cmake_autodetect(settings BUILD_TYPE ${TYPE})
                conan_cmake_install(
                        PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}
                        BUILD missing
                        SETTINGS ${settings}
                )
            endforeach()

            include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo_multi.cmake)
        else()
            conan_cmake_autodetect(CONAN_SETTINGS BUILD_TYPE ${CMAKE_BUILD_TYPE})
            conan_cmake_install(
                    PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}
                    BUILD missing
                    SETTINGS ${CONAN_SETTINGS}
            )
            #            include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
        endif()
    endif()
    message(VERBOSE "Conan Settings: ${CONAN_SETTINGS}")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_BINARY_DIR}")
    set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" PARENT_SCOPE)
endfunction()