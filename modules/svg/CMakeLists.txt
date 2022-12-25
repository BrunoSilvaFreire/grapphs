add_library(
        grapphs-svg
        include/grapphs/svg.h
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