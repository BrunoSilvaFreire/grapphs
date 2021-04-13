#include <grapphs/adjacency_list.h>
#include <grapphs/adjacency_matrix.h>
#include <grapphs/astar.h>
#include <grapphs/static_adjacency_matrix.h>
#include <gtest/gtest.h>
#include <random>

enum MyFlags {
    eA = 1 << 1,
    eB = 1 << 2,
    eC = 1 << 3
};

struct MyVertex {
public:
    MyFlags flags;

    bool operator==(const MyVertex &rhs) const {
        return flags == rhs.flags;
    }

    bool operator!=(const MyVertex &rhs) const {
        return !(rhs == *this);
    }
};

struct MyEdge {
    float weight;

    bool operator==(const MyEdge &rhs) const {
        return weight == rhs.weight;
    }

    bool operator!=(const MyEdge &rhs) const {
        return !(rhs == *this);
    }
};

typedef gpp::AdjacencyList<MyVertex, MyEdge> TestGraph;

#define NUM_GRAPH_ENTRIES 512
#define RANDOM_SEED "grapphs"

void populate(TestGraph &graph) {
    std::random_device device(RANDOM_SEED);

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
                edge.weight = *reinterpret_cast<float *>(&data);
                graph.connect(x, y, edge);
            }
        }
    }
}

struct Position {
    float x, y;
};


TEST(grapphs, graphCopies) {
    TestGraph graph;
    populate(graph);

    gpp::AdjacencyMatrix<MyVertex, MyEdge> copy(&graph);

    for (size_t i = 0; i < NUM_GRAPH_ENTRIES; i++) {
        MyVertex a = *graph.vertex(i);
        MyVertex b = *copy.vertex(i);

        ASSERT_TRUE(a == b);
    }
}


TEST(grapphs, astar_performance) {
    gpp::StaticAdjacencyMatrix<Position, bool, 4> graph;
    Position vert;
    vert.x = 0;
    vert.y = 0;
    *graph.vertex(0) = vert;

    gpp::astar<
            gpp::StaticAdjacencyMatrix<Position, bool, 4>,
            Position, bool
    >(graph, 0, 3, [](gpp::DefaultGraphIndex from, gpp::DefaultGraphIndex to) -> float {
        return 0.0F;
    });
}