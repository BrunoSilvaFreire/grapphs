cmake_minimum_required(VERSION 3.23)

find_package(Python 3.10 REQUIRED)

include(cmake/Grapphs.cmake)
include(cmake/GrapphsVersion.cmake)

grapphs_detect_version(GRAPPHS_VERSION)
message(STATUS "Grapphs version is: ${GRAPPHS_VERSION} (${GRAPPHS_VERSION_RAW})")

if(NOT GRAPPHS_VERSION STREQUAL "")
    project(grapphs LANGUAGES CXX VERSION "${GRAPPHS_VERSION}")
else()
    project(grapphs LANGUAGES CXX)
endif()

grapphs_bootstrap()

set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)
include(CMakePrintHelpers)
include(ExternalData)
include(cmake/GrapphsConan.cmake)

### Prepare install export
install(
        EXPORT grapphs
        DESTINATION .
        NAMESPACE grapphs::
)

### Prepare grapphs header-only library target
set(
        GRAPPHS_HEADERS

        include/grapphs/graph.h
        include/grapphs/graph_view.h
        include/grapphs/adjacency_list.h
        include/grapphs/adjacency_matrix.h
        include/grapphs/static_adjacency_matrix.h
        include/grapphs/algorithms/astar.h
        include/grapphs/algorithms/flood.h
        include/grapphs/algorithms/traversal.h
        include/grapphs/algorithms/bfs_traversal.h
        include/grapphs/algorithms/dfs_traversal.h
        include/grapphs/algorithms/rlo_traversal.h
)

add_library(
        grapphs INTERFACE
        ${GRAPPHS_HEADERS}
)

add_library(grapphs::grapphs ALIAS grapphs)

target_include_directories(
        grapphs
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_sources(
        grapphs
        PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS include
        FILES "${GRAPPHS_HEADERS}"
)

install(
        TARGETS grapphs
        EXPORT grapphs
        FILE_SET HEADERS
)

### Check for modules
option(GRAPPHS_COMPILE_TESTS "Create Unit Test executable?" ON)
option(GRAPPHS_COMPILE_GRAPHVIZ "Create graphviz support target?" ON)
option(GRAPPHS_COMPILE_SVG "Create svg target?" ON)
option(GRAPPHS_COMPILE_SAMPLES "Create samples targets?" ON)

grapphs_try_run_conan_install()

if(GRAPPHS_COMPILE_TESTS)



    set(GRAPPHS_MAZES_SIZES 3 5 10 15 25 35 50 100)
    add_custom_target(
            grapphs-generate-mazes
            COMMAND
            ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_BINDIR}/mazes
    )
    foreach(INDEX IN LISTS GRAPPHS_MAZES_SIZES)
        set(MAZE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/mazes/maze_${INDEX}.json)
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
    endforeach()
    add_library(
            grapphs-testlib
            tests/shared/grapphs/tests/geometry.h
            tests/shared/grapphs/tests/mazes.h
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
            tests/iteration.cpp
            tests/astar.cpp
            tests/adjacency_list.cpp
            tests/adjacency_matrix.cpp
    )
    add_dependencies(
            grapphs-tests
            grapphs-generate-mazes
    )
    target_link_libraries(
            grapphs-tests
            grapphs-testlib
    )
endif()

if(GRAPPHS_COMPILE_GRAPHVIZ)
    add_subdirectory(modules/graphviz)
endif()

if(GRAPPHS_COMPILE_SVG)
    add_subdirectory(modules/svg)
endif()

if(GRAPPHS_COMPILE_SAMPLES)
    add_subdirectory(samples/libosm)
    add_subdirectory(samples/osm_explorer)
endif()

include(CPack)