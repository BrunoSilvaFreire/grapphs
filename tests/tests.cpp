#include <grapphs/adjacency_list.h>
#include <grapphs/adjacency_matrix.h>
#include <grapphs/algorithms/flood.h>
#include <grapphs/algorithms/astar.h>
#include <grapphs/tests/mazes.h>
#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <cstdint>
#include <chrono>
#include <random>
#include <fstream>
#include <ostream>

enum MyFlags {
    eA = 1 << 1,
    eB = 1 << 2,
    eC = 1 << 3
};

struct MyVertex {
public:
    MyFlags flags;

    bool operator==(const MyVertex& rhs) const {
        return flags == rhs.flags;
    }

    bool operator!=(const MyVertex& rhs) const {
        return !(rhs == *this);
    }
};

struct MyEdge {
    float weight;

    bool operator==(const MyEdge& rhs) const {
        return weight == rhs.weight;
    }

    bool operator!=(const MyEdge& rhs) const {
        return !(rhs == *this);
    }
};

typedef gpp::AdjacencyList<MyVertex, MyEdge> TestGraph;

#define NUM_GRAPH_ENTRIES 512

void populate(TestGraph& graph) {
    std::random_device device;

    graph.reserve(NUM_GRAPH_ENTRIES);
    for (int i = 0; i < NUM_GRAPH_ENTRIES; ++i) {
        MyVertex vertex;
        vertex.flags = static_cast<MyFlags>(device());
        graph.push(vertex);
    }
    for (size_t x = 0; x < NUM_GRAPH_ENTRIES; ++x) {
        for (size_t y = 0; y < NUM_GRAPH_ENTRIES; ++y) {
            if (device()) {
                uint32_t data = device();
                MyEdge edge;
                edge.weight = *reinterpret_cast<float*>(&data);
                graph.connect(x, y, edge);
            }
        }
    }
}

struct Position {

    float x, y;

    float distance(const Position& other) const {
        float diffX = (other.x - x);
        float diffY = (other.y - y);
        return std::sqrt(diffX * diffX + diffY * diffY);
    }
};


TEST(grapphs, graph_copies) {
    TestGraph graph;
    populate(graph);

    gpp::AdjacencyMatrix<MyVertex, MyEdge> copy(&graph);

    for (size_t i = 0; i < NUM_GRAPH_ENTRIES; i++) {
        MyVertex a = *graph.vertex(i);
        MyVertex b = *copy.vertex(i);

        ASSERT_TRUE(a == b);
    }
}

TEST(grapphs, index_reuse) {
    gpp::AdjacencyList<int, bool> graph;
    graph.push(0);
    graph.push(0);
    graph.push(0);
    graph.remove(0);
    ASSERT_EQ(graph.push(2), 0);
}

TEST(grapphs, vertex_access) {
    gpp::AdjacencyList<int, bool> graph;
    std::mt19937 rand;
    std::uniform_int_distribution<> dist;
    for (int i = 0; i < std::numeric_limits<uint16_t>::max(); ++i) {
        int value = dist(rand);
        size_t index = graph.push(value);
        ASSERT_TRUE(*graph.vertex(index) == value);
        if (i % 2 == 0) {
            graph.remove(index);
        }
    }
}


#define ASTAR_RADIUS 20
#define INDEXOF(x, y)  x + (y) * ASTAR_RADIUS
TEST(grapphs, astar_performance) {
    gpp::AdjacencyList<Position, MyEdge> graph;
    for (int x = 0; x < ASTAR_RADIUS; ++x) {
        for (int y = 0; y < ASTAR_RADIUS; ++y) {
            Position pos{};
            pos.x = x;
            pos.y = y;
            graph.push(pos);
        }
    }
    MyEdge prototype{};
    prototype.weight = 1;
    for (int x = 0; x < ASTAR_RADIUS; ++x) {
        for (int y = 0; y < ASTAR_RADIUS; ++y) {
            uint32_t index = INDEXOF(x, y);
            if (x != 0) {
                graph.connect(index, INDEXOF(x - 1, y), prototype);
            }
            if (y != 0) {
                graph.connect(index, INDEXOF(x, y - 1), prototype);
            }
            if (x != ASTAR_RADIUS - 1) {
                graph.connect(index, INDEXOF(x + 1, y), prototype);
            }
            if (y != ASTAR_RADIUS - 1) {
                graph.connect(index, INDEXOF(x, y + 1), prototype);
            }
        }
    }

    gpp::GraphPath<size_t> path;
#define RUN_COUNT 10000
    double meanTime = 0;
    const float frameTime = 1.0F / 60.0F;
    for (int i = 0; i < RUN_COUNT; ++i) {
        auto begin = std::chrono::high_resolution_clock::now();
        path = gpp::astar<gpp::AdjacencyList<Position, MyEdge>>(
            graph,
            0,
            (ASTAR_RADIUS * ASTAR_RADIUS) - 1,
            [&](gpp::DefaultGraphIndex from, gpp::DefaultGraphIndex to) -> float {
                Position* fromPos = graph.vertex(from);
                Position* toPos = graph.vertex(to);
                return fromPos->distance(*toPos);
            },
            [](gpp::DefaultGraphIndex from, gpp::DefaultGraphIndex to, const MyEdge& e) -> float {
                return e.weight;
            }
        );
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time = end - begin;

        meanTime += time.count();
    }
    meanTime /= RUN_COUNT;
    std::cout << "AStar for " << ASTAR_RADIUS << " radius took " << meanTime << "s on average (limit: " << frameTime
              << "s, "
              << RUN_COUNT << " runs)"
              << std::endl;
    ASSERT_LT(meanTime, frameTime);

}


TEST(grapphs, maze_astar) {
    gpp::test_mazes([](gpp::Maze& m) {
        auto maze = m.getGraph();
        auto start = m.getStart();
        auto end = m.getEnd();
        std::size_t size = m.getSize();
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
    gpp::test_mazes([](gpp::Maze& m) {
        auto maze = m.getGraph();
        auto start = m.getStart();
        auto end = m.getEnd();
        auto shortestPath = m.getShortestPath();
        std::size_t size = m.getSize();
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
                    GTEST_LOG_(ERROR) << from << " is not the same as " << shortestPath[index] << " @ " << index;
                }
                ASSERT_TRUE(isSame);
                index++;
            }
        );
    });
}


TEST(grapphs, maze_flood) {
    gpp::test_mazes([](gpp::Maze& m) {
        auto maze = m.getGraph();
        auto start = m.getStart();
        auto end = m.getEnd();
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
            [&](size_t from, size_t to) {}
        );
        ASSERT_TRUE(pending.empty());
    });
}