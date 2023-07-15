 grapphs_run_conan_install(${CMAKE_CURRENT_SOURCE_DIR})

 set(GRAPPHS_MAZES_SIZES 3 5 10 15 25 35 50 100)
 add_custom_target(
         grapphs-generate-mazes
         COMMAND
         ${CMAKE_COMMAND} -E make_directory ${CMAKE_INSTALL_BINDIR}/mazes
 )
 foreach(INDEX IN LISTS GRAPPHS_MAZES_SIZES)
     set(MAZE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/mazes/maze_${INDEX}.json)
     add_custom_command(
             TARGET
             grapphs-generate-mazes
             COMMAND
             ${Python3_EXECUTABLE}
             ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/maze_gen.py
             --size ${INDEX}
             --monteMazes 10
             --monteAttempts 10
             --output ${MAZE_OUTPUT}
             --reuse true
             BYPRODUCTS ${MAZE_OUTPUT}
             COMMENT "Generating maze ${MAZE_OUTPUT} of size ${INDEX}..."
     )
 endforeach()

 message("Module path: ${CMAKE_MODULE_PATH}")
 find_package(nlohmann_json REQUIRED)
 find_package(GTest REQUIRED)
 add_library(
         grapphs-testlib
         shared/grapphs/tests/geometry.h
         shared/grapphs/tests/mazes.h
         shared/grapphs/tests/mazes.cpp
         shared/grapphs/tests/traversal_order.h
         shared/grapphs/tests/traversal_order.cpp
 )
 target_link_libraries(
         grapphs-testlib
         grapphs
         nlohmann_json::nlohmann_json
         GTest::gtest
         GTest::gtest_main
 )
 target_include_directories(
         grapphs-testlib
         PUBLIC
         shared
 )
 add_executable(
         grapphs-tests
         tests.cpp
         traversal.cpp
         maze.cpp
         iteration.cpp
         astar.cpp
         adjacency_list.cpp
         adjacency_matrix.cpp
 )

 add_dependencies(
         grapphs-tests
         grapphs-generate-mazes
 )
 target_link_libraries(
         grapphs-tests
         grapphs-testlib
 )