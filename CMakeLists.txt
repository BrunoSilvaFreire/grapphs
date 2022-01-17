cmake_minimum_required(VERSION 3.19)
project(grapphs)
set(CMAKE_CXX_STANDARD 17)
include(GNUInstallDirs)
add_library(
        grapphs INTERFACE
        include/grapphs/graph.h
        include/grapphs/adjacency_list.h
        include/grapphs/adjacency_matrix.h
        include/grapphs/static_adjacency_matrix.h
        include/grapphs/algorithms/astar.h
        include/grapphs/algorithms/flood.h
        include/grapphs/algorithms/traversal.h)
target_include_directories(
        grapphs
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
include(CMakePrintHelpers)

set_target_properties(
        grapphs
        PROPERTIES
        PUBLIC_HEADER $<TARGET_PROPERTY:grapphs,INTERFACE_INCLUDE_DIRECTORIES>
)

install(
        TARGETS grapphs
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

option(GRAPPHS_COMPILE_TESTS "Create Unit Test executable?" ON)
option(GRAPPHS_COMPILE_VISUALIZATION "Create visualization target?" ON)
if (GRAPPHS_COMPILE_TESTS)
    if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/release/0.17/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
    endif ()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

    conan_cmake_configure(
            REQUIRES
            gtest/1.10.0
            nlohmann_json/3.10.5
            GENERATORS cmake
    )
    conan_cmake_autodetect(CONAN_SETTINGS BUILD_TYPE ${CMAKE_BUILD_TYPE})
    conan_cmake_install(
            PATH_OR_REFERENCE .
            BUILD missing
            SETTINGS ${CONAN_SETTINGS}
    )
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)

    find_package(Python REQUIRED)


    set(GRAPPHS_MAZES_SIZES 3 5 10 15 25 35 50 100)
    add_custom_target(
            grapphs_generate_mazes
            COMMAND
            ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_BINDIR}/mazes
    )
    foreach (INDEX IN LISTS GRAPPHS_MAZES_SIZES)
        set(MAZE_OUTPUT ${CMAKE_INSTALL_BINDIR}/mazes/maze_${INDEX}.json)
        add_custom_command(
                TARGET
                grapphs_generate_mazes
                COMMAND
                ${Python_EXECUTABLE}
                ${CMAKE_SOURCE_DIR}/scripts/maze_gen.py
                --size ${INDEX}
                --monteMazes 10
                --monteAttempts 10
                --output ${MAZE_OUTPUT}
                --reuse true
                BYPRODUCTS ${MAZE_OUTPUT}
                COMMENT "Generating maze of size ${INDEX}..."

        )
    endforeach ()
    add_library(
            grapphs_testlib
            tests/shared/grapphs
            tests/shared/grapphs/tests/mazes.cpp)
    target_link_libraries(
            grapphs_testlib
            grapphs
            CONAN_PKG::nlohmann_json
            CONAN_PKG::gtest
    )
    target_include_directories(
            grapphs_testlib
            PUBLIC
            tests/shared
    )
    add_executable(
            grapphs_tests
            tests/tests.cpp
            tests/traversal.cpp
            tests/maze.cpp
    )
    add_dependencies(
            grapphs_tests
            grapphs_generate_mazes
    )
    target_link_libraries(
            grapphs_tests
            grapphs_testlib
    )
endif ()
if (GRAPPHS_COMPILE_VISUALIZATION)
    add_subdirectory(visualization)
endif ()