add_executable(
        grapphs_osm_explorer
        main.cpp
)

target_link_libraries(
        grapphs_osm_explorer
        PUBLIC
        grapphs_libosm
)