add_library(
        grapphs-libosm
        include/grapphs/osm/way.h
        src/grapphs/osm/way.cpp
        include/grapphs/osm/parse.h src/grapphs/osm/parse.cpp)

find_package(readosm REQUIRED)
target_link_libraries(
        grapphs-libosm
        PUBLIC
        grapphs
        readosm::readosm
)

target_include_directories(
        grapphs-libosm
        PUBLIC
        include
)