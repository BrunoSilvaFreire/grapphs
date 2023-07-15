cmake_minimum_required(VERSION 3.23)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

include(cmake/Grapphs.cmake)
include(cmake/GrapphsVersion.cmake)

if(NOT GRAPPHS_VERSION)
    grapphs_detect_version(GRAPPHS_VERSION)
endif()

message(STATUS "Grapphs version is: ${GRAPPHS_VERSION} (${GRAPPHS_VERSION_RAW})")

if(NOT GRAPPHS_VERSION STREQUAL "")
    project(grapphs LANGUAGES C CXX VERSION "${GRAPPHS_VERSION}")
else()
    project(grapphs LANGUAGES C CXX)
endif()

grapphs_bootstrap()

set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)
include(CMakePrintHelpers)
include(ExternalData)
include(cmake/GrapphsConan.cmake)

if(NOT CONAN_EXPORTED)
    ### Prepare install export
    install(
            EXPORT grapphs
            DESTINATION .
            NAMESPACE grapphs::
    )
endif()

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

grapphs_run_conan_install(${CMAKE_SOURCE_DIR})

if(GRAPPHS_COMPILE_TESTS)
   add_subdirectory(tests)
endif()

if(GRAPPHS_COMPILE_GRAPHVIZ)
    add_subdirectory(modules/graphviz)
endif()

if(GRAPPHS_COMPILE_SVG)
    add_subdirectory(modules/svg)
endif()

if(GRAPPHS_COMPILE_SAMPLES)
    add_subdirectory(samples)
endif()

include(CPack)