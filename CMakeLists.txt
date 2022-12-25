cmake_minimum_required(VERSION 3.19)
project(grapphs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)
include(CMakePrintHelpers)
include(ExternalData)

add_library(
        grapphs INTERFACE
        include/grapphs/graph.h
        include/grapphs/adjacency_list.h
        include/grapphs/adjacency_matrix.h
        include/grapphs/static_adjacency_matrix.h
        include/grapphs/algorithms/astar.h
        include/grapphs/algorithms/flood.h
        include/grapphs/algorithms/traversal.h
        include/grapphs/algorithms/bfs_traversal.h
        include/grapphs/algorithms/dfs_traversal.h
        include/grapphs/algorithms/rlo_traversal.h
        samples/osm_explorer/main.cpp
)

add_library(grapphs::grapphs ALIAS grapphs)

target_include_directories(
        grapphs
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

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
option(GRAPPHS_COMPILE_GRAPHVIZ "Create graphviz support target?" ON)
option(GRAPPHS_COMPILE_SVG "Create svg target?" ON)
option(GRAPPHS_COMPILE_SAMPLES "Create samples targets?" ON)

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
            gtest/1.11.0
            nlohmann_json/3.10.5
            readosm/1.1.0a
            GENERATORS cmake
    )
    message("Settings: ${CONAN_SETTINGS}")
    if ("${CONAN_SETTINGS}" STREQUAL "")
        conan_cmake_autodetect(CONAN_SETTINGS BUILD_TYPE ${CMAKE_BUILD_TYPE})
    endif ()
    conan_cmake_install(
            PATH_OR_REFERENCE .
            BUILD missing
            SETTINGS ${CONAN_SETTINGS}
    )
    include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)

    find_package(Python REQUIRED)


    set(GRAPPHS_MAZES_SIZES 3 5 10 15 25 35 50 100)
    add_custom_target(
            grapphs-generate-mazes
            COMMAND
            ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_BINDIR}/mazes
    )
    foreach (INDEX IN LISTS GRAPPHS_MAZES_SIZES)
        set(MAZE_OUTPUT ${CMAKE_INSTALL_BINDIR}/mazes/maze_${INDEX}.json)
        add_custom_command(
                TARGET
                grapphs-generate-mazes
                COMMAND
                ${Python_EXECUTABLE}
                ${CMAKE_CURRENT_SOURCE_DIR}/scripts/maze_gen.py
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
            grapphs-testlib
            tests/shared/grapphs
            tests/shared/grapphs/tests/mazes.cpp
            tests/shared/grapphs/tests/traversal_order.h
            tests/shared/grapphs/tests/traversal_order.cpp
    )
    target_link_libraries(
            grapphs-testlib
            grapphs
            CONAN_PKG::nlohmann_json
            CONAN_PKG::gtest
    )
    target_include_directories(
            grapphs-testlib
            PUBLIC
            tests/shared
    )
    add_executable(
            grapphs-tests
            tests/tests.cpp
            tests/traversal.cpp
            tests/maze.cpp
    )
    add_dependencies(
            grapphs-tests
            grapphs-generate-mazes
    )
    target_link_libraries(
            grapphs-tests
            grapphs-testlib
    )
endif ()

if (GRAPPHS_COMPILE_GRAPHVIZ)
    add_subdirectory(modules/graphviz)
endif ()

if (GRAPPHS_COMPILE_SVG)
    add_subdirectory(modules/svg)
endif ()

if (GRAPPHS_COMPILE_SAMPLES)
    add_subdirectory(samples/libosm)
    add_subdirectory(samples/osm_explorer)
endif ()