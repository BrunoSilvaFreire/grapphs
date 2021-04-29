#include <grapphs/adjacency_list.h>
#include <grapphs/adjacency_matrix.h>
#include <grapphs/astar.h>
#include <gtest/gtest.h>
#include <chrono>

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

    float distance(const Position &other) const {
        float diffX = (other.x - x);
        float diffY = (other.y - y);
        return std::sqrt(diffX * diffX + diffY * diffY);
    }
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

TEST(grapphs, removeAtIndex) {
    gpp::AdjacencyList<int, bool> graph;
    graph.push(0);
    graph.push(0);
    graph.push(0);
    graph.remove(0);
    ASSERT_EQ(graph.push(2), 0);
}


#define ASTAR_RADIUS 20
#define INDEXOF(x, y)  x + (y) * ASTAR_RADIUS
TEST(grapphs, astar_performance) {
    gpp::AdjacencyList<Position, MyEdge> graph;
    for (int x = 0; x < ASTAR_RADIUS; ++x) {
        for (int y = 0; y < ASTAR_RADIUS; ++y) {
            Position pos;
            pos.x = x;
            pos.y = y;
            graph.push(pos);
        }
    }
    MyEdge prototype;
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

    gpp::GraphPath<gpp::AdjacencyList<Position, MyEdge>> path;
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
                    Position *fromPos = graph.vertex(from);
                    Position *toPos = graph.vertex(to);
                    return fromPos->distance(*toPos);
                },
                [](gpp::DefaultGraphIndex from, gpp::DefaultGraphIndex to, const MyEdge &e) -> float {
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