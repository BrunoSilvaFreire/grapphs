#include <grapphs/adjacency_matrix.h>
#include <grapphs/algorithms/flood.h>
#include <grapphs/algorithms/astar.h>
#include <grapphs/tests/mazes.h>
#include <grapphs/tests/string_helper.h>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <cstdint>
#include <chrono>
#include <random>
#include <fstream>
#include <ostream>

TEST(grapphs, maze_astar) {
    gpp::test_mazes([](gpp::maze& m) {
        auto maze = m.get_graph();
        auto start = m.get_start();
        auto end = m.get_end();
        std::size_t size = m.get_size();
        auto result = gpp::astar(
            maze, start, end,
            [&](size_t from, size_t to) -> float {
                float fx, fy;
                float tx, ty;
                fx = static_cast<float> (from % size);
                tx = static_cast<float>(to % size);
                fy = static_cast<float>(from / size);
                ty = static_cast<float>(to / size);
                float dx = fx - tx;
                float dy = fy - ty;
                return std::sqrt(
                    dx * dx + dy * dy
                );
            },
            [](size_t from, size_t to, const int& value) {
                return value;
            }
        );
        GTEST_LOG_(INFO) << "Path from " << start << " to " << end << ":";
        std::stringstream path;
        size_t previous = start;
        path << start;
        result.for_each(
            [&](size_t from, size_t to) {
                ASSERT_TRUE(from == previous);
                ASSERT_TRUE(maze.edge(from, to) != nullptr);
                path << "-" << to;
                previous = to;
            }
        );
        GTEST_LOG_(INFO) << path.str();

    });
}

TEST(grapphs, maze_shortest_path) {
    gpp::test_mazes([](gpp::maze& m) {
        auto maze = m.get_graph();
        auto start = m.get_start();
        auto end = m.get_end();
        auto shortestPath = m.get_shortest_path();
        std::size_t size = m.get_size();
        auto result = gpp::astar(
            maze, start, end,
            [&](size_t from, size_t to) -> float {
                float fx, fy;
                float tx, ty;
                fx = static_cast<float> (from % size);
                tx = static_cast<float>(to % size);
                fy = static_cast<float>(from / size);
                ty = static_cast<float>(to / size);
                float dx = fx - tx;
                float dy = fy - ty;
                return std::sqrt(
                    dx * dx + dy * dy
                );
            },
            [](size_t from, size_t to, int value) {
                return value;
            }
        );
        std::size_t index = 0;
        result.for_each(
            [&](size_t from, size_t to) {
                bool isSame = from == shortestPath[index];
                if (!isSame) {
                    GTEST_LOG_(ERROR) << from << " is not the same as " << shortestPath[index]
                                      << " @ " << index;
                }
                ASSERT_TRUE(isSame);
                index++;
            }
        );
    });
}

TEST(grapphs, maze_flood) {
    gpp::test_mazes([](gpp::maze& m) {
        auto maze = m.get_graph();
        auto start = m.get_start();
        auto end = m.get_end();
        std::stringstream report;
        std::set<size_t> pending;
        for (size_t i = 0; i < maze.size(); ++i) {
            pending.emplace(i);
        }
        gpp::flood(
            maze,
            {start},
            [&](size_t index) {
                pending.erase(index);
            },
            [&](size_t from, size_t to) {
            }
        );
        ASSERT_TRUE(pending.empty()) << "Pending vertices " << gpp::tests::join_string(pending) << " were not visited.";
    });
}