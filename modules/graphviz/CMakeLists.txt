set(
        GRAPPHS_GRAPHVIZ_HEADERS

        include/grapphs/dot.h
        include/grapphs/graph_writer.h
)
add_library(
        grapphs-graphviz
        ${GRAPPHS_GRAPHVIZ_HEADERS}
        src/grapphs/dot.cpp
)

add_library(grapphs::graphviz ALIAS grapphs-graphviz)

target_include_directories(
        grapphs-graphviz
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(
        grapphs-graphviz
        PROPERTIES
        LINKER_LANGUAGE CXX
)

target_sources(
        grapphs-graphviz
        PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS include
        FILES "${GRAPPHS_GRAPHVIZ_HEADERS}"
)

install(
        TARGETS grapphs-graphviz
        EXPORT grapphs
        FILE_SET HEADERS DESTINATION graphviz/${CMAKE_INSTALL_INCLUDEDIR}
        ARCHIVE DESTINATION graphviz/${CMAKE_INSTALL_LIBDIR}
)

target_link_libraries(
        grapphs-graphviz
        grapphs
)
option(GRAPPHS_COMPILE_VISUALIZATION_TESTS "Create visualization target tests?" ON)

if(GRAPPHS_COMPILE_TESTS AND GRAPPHS_COMPILE_VISUALIZATION_TESTS)
    add_executable(
            grapphs-graphviz-tests
            tests/tests.cpp
    )
    target_link_libraries(
            grapphs-graphviz-tests
            grapphs-graphviz
            grapphs-testlib
    )
    add_dependencies(
            grapphs-graphviz-tests
            grapphs-generate-mazes
    )
    add_custom_command(
            TARGET grapphs-graphviz-tests
            COMMAND ${CMAKE_COMMAND}
            -E copy_directory
            $<TARGET_PROPERTY:grapphs,BINARY_DIR>/bin/mazes
            ${CMAKE_CURRENT_BINARY_DIR}/bin/mazes
    )
endif()