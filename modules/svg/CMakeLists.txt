set(
        GRAPPHS_SVG_HEADERS
        include/grapphs/svg.h
)

add_library(
        grapphs-svg
        ${GRAPPHS_SVG_HEADERS}
        src/grapphs/svg.cpp
)

target_include_directories(
        grapphs-svg
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
        grapphs-svg
        grapphs
)
target_sources(
        grapphs-svg
        PUBLIC
            FILE_SET HEADERS
                TYPE HEADERS
                BASE_DIRS include
                FILES "${GRAPPHS_SVG_HEADERS}"
)

install(
        TARGETS grapphs-svg
        EXPORT grapphs
        FILE_SET HEADERS DESTINATION svg/${CMAKE_INSTALL_INCLUDEDIR}
        ARCHIVE DESTINATION svg/${CMAKE_INSTALL_LIBDIR}
)

option(GRAPPHS_COMPILE_SVG_TESTS "Create svg target tests?" ON)

if(GRAPPHS_COMPILE_TESTS AND GRAPPHS_COMPILE_SVG_TESTS)
    add_executable(
            grapphs-svg-tests
            tests/tests.cpp
    )
    target_link_libraries(
            grapphs-svg-tests
            grapphs-svg
            grapphs-testlib
    )
    add_dependencies(
            grapphs-svg-tests
            grapphs-generate-mazes
    )
    grapphs_set_target_output_directory_same_as(grapphs-svg-tests grapphs-tests)
endif()