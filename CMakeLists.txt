cmake_minimum_required(VERSION 3.0)
project(grapphs)
set(CMAKE_CXX_STANDARD 17)
add_library(
        grapphs INTERFACE
)

set_target_properties(
        grapphs
        PROPERTIES
        LINKER_LANGUAGE CXX
        target_sources
        "include/grapphs/graph.h;
        include/grapphs/adjacency_list.h;
        include/grapphs/adjacency_matrix.h;
        include/grapphs/static_adjacency_matrix.h;
        include/grapphs/astar.h"
)
target_include_directories(
        grapphs
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
option(GRAPPHS_COMPILE_TESTS "Create Unit Test executable?" ON)
if (GRAPPHS_COMPILE_TESTS)

    if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
    endif ()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

    conan_cmake_configure(
            REQUIRES gtest/1.10.0
            GENERATORS cmake_find_package
    )
    conan_cmake_autodetect(settings BUILD_TYPE ${CMAKE_BUILD_TYPE})
    conan_cmake_install(PATH_OR_REFERENCE .
            BUILD missing
            REMOTE conan-center
            SETTINGS ${settings})

    find_package(GTest REQUIRED)

    add_executable(
            grapphs_tests
            tests/tests.cpp
    )
    target_link_libraries(
            grapphs_tests
            grapphs
            GTest::GTest
    )
endif ()
