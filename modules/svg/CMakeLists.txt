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
