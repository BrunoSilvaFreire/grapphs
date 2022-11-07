add_library(
        grapphs_libosm
        include/grapphs/osm/way.h
        src/grapphs/osm/way.cpp
        include/grapphs/osm/parse.h src/grapphs/osm/parse.cpp)

target_link_libraries(
        grapphs_libosm
        PUBLIC
        grapphs
        grapphs_visualization
        CONAN_PKG::readosm
)

target_include_directories(
        grapphs_libosm
        PUBLIC
        include
)