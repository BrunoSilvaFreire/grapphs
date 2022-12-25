add_executable(
        grapphs-osm-explorer
        main.cpp
        curitiba.xml.osm
)

target_link_libraries(
        grapphs-osm-explorer
        PUBLIC
        grapphs-libosm
        grapphs-svg
)

add_custom_command(
        OUTPUT curitiba.xml.osm
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/planet/curitiba.xml.osm
        $<TARGET_FILE_DIR:grapphs-osm-explorer>/curitiba.xml.osm
        COMMENT "Copying ${CMAKE_CURRENT_SOURCE_DIR}/planet/curitiba.xml.osm to build directory"
        DEPENDS planet/curitiba.xml.osm
)