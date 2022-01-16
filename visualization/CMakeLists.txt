add_library(
        grapphs_visualization
        include/grapphs/dot.h
        src/grapphs/dot.cpp
)
target_include_directories(
        grapphs_visualization
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
set_target_properties(
        grapphs_visualization
        PROPERTIES
        LINKER_LANGUAGE CXX
)
target_link_libraries(
        grapphs_visualization
        grapphs
)
option(GRAPPHS_COMPILE_VISUALIZATION_TESTS "Create visualization target tests?" ON)

if (GRAPPHS_COMPILE_VISUALIZATION_TESTS)
    add_executable(
            grapphs_visualization_tests
            tests/tests.cpp
    )
    target_link_libraries(
            grapphs_visualization_tests
            grapphs_visualization
            grapphs_testlib
    )
    add_dependencies(
            grapphs_visualization_tests
            grapphs_generate_mazes
    )
    add_custom_command(
            TARGET grapphs_visualization_tests
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/mazes ${CMAKE_CURRENT_BINARY_DIR}/mazes
    )
endif ()