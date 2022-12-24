#include <grapphs/adjacency_list.h>
#include <grapphs/adjacency_matrix.h>
#include <grapphs/dot.h>
#include "grapphs/algorithms/astar.h"
#include <gtest/gtest.h>
#include <chrono>

#include <random>
#include <fstream>
#include <ostream>
#include <nlohmann/json.hpp>
#include <grapphs/tests/mazes.h>

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

    friend std::ostream& operator<<(std::ostream& os, const MyVertex& vertex) {
        os << "flags: " << vertex.flags;
        return os;
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

    friend std::ostream& operator<<(std::ostream& os, const MyEdge& edge) {
        os << "weight: " << edge.weight;
        return os;
    }
};

typedef gpp::AdjacencyList<MyVertex, MyEdge> TestGraph;

#define NUM_GRAPH_ENTRIES 10

void populate(TestGraph& graph) {
    std::random_device device;
    std::uniform_real_distribution<float> distribution;
    graph.reserve(NUM_GRAPH_ENTRIES);
    for (int i = 0; i < NUM_GRAPH_ENTRIES; ++i) {
        MyVertex vertex{};
        vertex.flags = static_cast<MyFlags>(device() % 3);
        graph.push(vertex);
    }
    constexpr float connectionThreshold = 0.125F;
    for (size_t x = 0; x < NUM_GRAPH_ENTRIES; ++x) {
        for (size_t y = 0; y < NUM_GRAPH_ENTRIES; ++y) {
            if (x != y && distribution(device) < connectionThreshold) {
                MyEdge edge{};
                edge.weight = distribution(device);
                graph.connect(x, y, edge);
            }
        }
    }
}


TEST(grapphs_visualization, to_dot) {
    TestGraph graph;
    populate(graph);
    auto outputPath = std::filesystem::current_path() / "output.dot";
    GTEST_LOG_(INFO) << "Writing dot to: " << outputPath;
    gpp::save_to_dot(graph, outputPath);
}


TEST(grapphs_visualization, maze_to_dot) {
    TestGraph graph;
    std::filesystem::path jsonPath = std::filesystem::current_path() / "maze.json";
    gpp::test_mazes([](gpp::Maze& m) {
        auto maze = m.getGraph();
        auto start = m.getStart();
        auto end = m.getEnd();
        std::stringstream name;
        name << "maze_" << m.getSize() << ".dot";
        gpp::save_to_dot(maze, std::filesystem::current_path() / name.str());
    });

}