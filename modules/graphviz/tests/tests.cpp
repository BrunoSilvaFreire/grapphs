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

enum my_flags {
    A = 1 << 1,
    B = 1 << 2,
    C = 1 << 3
};

struct my_vertex {
public:
    my_flags flags;

    bool operator==(const my_vertex& rhs) const {
        return flags == rhs.flags;
    }

    bool operator!=(const my_vertex& rhs) const {
        return !(rhs == *this);
    }

    friend std::ostream& operator<<(std::ostream& os, const my_vertex& vertex) {
        os << "flags: " << vertex.flags;
        return os;
    }
};

struct my_edge {
    float weight;

    bool operator==(const my_edge& rhs) const {
        return weight == rhs.weight;
    }

    bool operator!=(const my_edge& rhs) const {
        return !(rhs == *this);
    }

    friend std::ostream& operator<<(std::ostream& os, const my_edge& edge) {
        os << "weight: " << edge.weight;
        return os;
    }
};

typedef gpp::adjacency_list<my_vertex, my_edge> test_graph;

#define NUM_GRAPH_ENTRIES 10

void populate(test_graph& graph) {
    std::random_device device;
    std::uniform_real_distribution<float> distribution;
    graph.reserve(NUM_GRAPH_ENTRIES);
    for (int i = 0; i < NUM_GRAPH_ENTRIES; ++i) {
        my_vertex vertex{};
        vertex.flags = static_cast<my_flags>(device() % 3);
        graph.push(vertex);
    }
    constexpr float connectionThreshold = 0.125F;
    for (size_t x = 0; x < NUM_GRAPH_ENTRIES; ++x) {
        for (size_t y = 0; y < NUM_GRAPH_ENTRIES; ++y) {
            if (x != y && distribution(device) < connectionThreshold) {
                my_edge edge{};
                edge.weight = distribution(device);
                graph.connect(x, y, edge);
            }
        }
    }
}

TEST(grapphs_visualization, to_dot) {
    test_graph graph;
    populate(graph);
    auto outputPath = std::filesystem::current_path() / "output.dot";
    GTEST_LOG_(INFO) << "Writing dot to: " << outputPath;
    gpp::save_to_dot(graph, outputPath);
}

TEST(grapphs_visualization, maze_to_dot) {
    test_graph graph;
    std::filesystem::path jsonPath = std::filesystem::current_path() / "maze.json";
    gpp::test_mazes([](gpp::maze& m) {
        auto maze = m.get_graph();
        auto start = m.get_start();
        auto end = m.get_end();
        std::stringstream name;
        name << "maze_" << m.get_size() << ".dot";
        gpp::save_to_dot(maze, std::filesystem::current_path() / name.str());
    });

}