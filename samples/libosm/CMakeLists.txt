add_library(
        grapphs-libosm
        include/grapphs/osm/way.h
        src/grapphs/osm/way.cpp
        include/grapphs/osm/parse.h src/grapphs/osm/parse.cpp)

target_link_libraries(
        grapphs-libosm
        PUBLIC
        grapphs
        CONAN_PKG::readosm
)

target_include_directories(
        grapphs-libosm
        PUBLIC
        include
)