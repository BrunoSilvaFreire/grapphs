#include <grapphs/adjacency_list.h>
#include <grapphs/adjacency_matrix.h>
#include <grapphs/tests/geometry.h>
#include <gtest/gtest.h>
#include <chrono>
#include <random>

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
};

struct my_edge {
    float weight;

    bool operator==(const my_edge& rhs) const {
        return weight == rhs.weight;
    }

    bool operator!=(const my_edge& rhs) const {
        return !(rhs == *this);
    }
};


#define NUM_GRAPH_ENTRIES 512

template<typename t_graph>
void populate(t_graph& graph);

template<>
void populate(gpp::adjacency_list<my_vertex, my_edge>& graph) {
    std::random_device device;

    graph.reserve(NUM_GRAPH_ENTRIES);
    for (int i = 0; i < NUM_GRAPH_ENTRIES; ++i) {
        my_vertex vertex{};
        vertex.flags = static_cast<my_flags>(device());
        graph.push(vertex);
    }
    for (size_t x = 0; x < NUM_GRAPH_ENTRIES; ++x) {
        for (size_t y = 0; y < NUM_GRAPH_ENTRIES; ++y) {
            if (device()) {
                uint32_t data = device();
                my_edge edge;
                edge.weight = *reinterpret_cast<float*>(&data);
                graph.connect(x, y, edge);
            }
        }
    }
}

template<>
void populate(gpp::adjacency_matrix<my_vertex, my_edge>& graph) {
    std::random_device device;

    for (int i = 0; i < NUM_GRAPH_ENTRIES; ++i) {
        my_vertex vertex{};
        vertex.flags = static_cast<my_flags>(device());
        graph[i] = vertex;
    }

    for (size_t x = 0; x < NUM_GRAPH_ENTRIES; ++x) {
        for (size_t y = 0; y < NUM_GRAPH_ENTRIES; ++y) {
            if (device()) {
                uint32_t data = device();
                my_edge edge;
                edge.weight = *reinterpret_cast<float*>(&data);
                graph.connect(x, y, edge);
            }
        }
    }
}


struct Position {

    float x, y;

    float distance(const Position& other) const {
        return ::distance(x, y, other.x, other.y);
    }
};

template<typename t_graph>
void test_copy(t_graph& graph){
    populate(graph);

    gpp::adjacency_matrix<my_vertex, my_edge> copy(&graph);

    for (size_t i = 0; i < NUM_GRAPH_ENTRIES; i++) {
        my_vertex a = *graph.vertex(i);
        my_vertex b = *copy.vertex(i);

        ASSERT_TRUE(a == b);
    }
}

TEST(grapphs, adjacency_list_copy) {
    gpp::adjacency_list<my_vertex, my_edge> graph;
    test_copy(graph);
}


TEST(grapphs, adjacency_matrix_copy) {
    gpp::adjacency_matrix<my_vertex, my_edge> graph(NUM_GRAPH_ENTRIES);
    test_copy(graph);
}

TEST(grapphs, index_reuse) {
    gpp::adjacency_list<my_vertex, bool> graph;
    my_vertex a, b, c, d{};
    a.flags = my_flags::A;
    b.flags = my_flags::B;
    c.flags = my_flags::C;
    auto first = graph.push(a);
    graph.push(b);
    auto second = graph.push(std::move(c));
    graph.remove(first);
    ASSERT_EQ(graph.push(d), first);
    graph.remove(second);
    ASSERT_EQ(graph.push(std::move(b)), second);
}

TEST(grapphs, adjacency_list_vertex_access) {
    gpp::adjacency_list<int, bool> graph;
    std::mt19937 rand;
    std::uniform_int_distribution<> dist;
    for (int i = 0; i < std::numeric_limits<uint16_t>::max(); ++i) {
        int value = dist(rand);
        size_t index = graph.push(value);
        ASSERT_TRUE(*graph.vertex(index) == value);
    }
}

TEST(grapphs, adjacency_matrix_vertex_access) {
    gpp::adjacency_matrix<int, int> graph(std::numeric_limits<uint8_t>::max());
    std::mt19937 rand;
    std::uniform_int_distribution<> dist;
    const decltype(graph)& constGraph = graph;
    for (int i = 0; i < std::numeric_limits<uint8_t>::max(); ++i) {
        int value = dist(rand);
        graph[i] = value;
        ASSERT_TRUE(graph[i] == value);
        const int asConst = *constGraph.vertex(i);
        ASSERT_TRUE(asConst == value);
    }
}

#define ASTAR_RADIUS 20

inline int index_of(int x, int y) { return x + (y) * ASTAR_RADIUS; }

/*
TEST(grapphs, astar_performance) {
    gpp::adjacency_list<Position, my_edge> graph;
    for (int x = 0; x < ASTAR_RADIUS; ++x) {
        for (int y = 0; y < ASTAR_RADIUS; ++y) {
            Position pos{};
            pos.x = x;
            pos.y = y;
            graph.push(pos);
        }
    }
    my_edge prototype{};
    prototype.weight = 1;
    for (int x = 0; x < ASTAR_RADIUS; ++x) {
        for (int y = 0; y < ASTAR_RADIUS; ++y) {
            uint32_t index = index_of(x, y);
            if (x != 0) {
                graph.connect(index, index_of(x - 1, y), prototype);
            }
            if (y != 0) {
                graph.connect(index, index_of(x, y - 1), prototype);
            }
            if (x != ASTAR_RADIUS - 1) {
                graph.connect(index, index_of(x + 1, y), prototype);
            }
            if (y != ASTAR_RADIUS - 1) {
                graph.connect(index, index_of(x, y + 1), prototype);
            }
        }
    }

    gpp::graph_path<size_t> path;
#define RUN_COUNT 10000
    double meanTime = 0;
    const float frameTime = 1.0F / 60.0F;
    for (int i = 0; i < RUN_COUNT; ++i) {
        auto begin = std::chrono::high_resolution_clock::now();
        path = gpp::astar<gpp::adjacency_list<Position, my_edge>>(
            graph,
            0,
            (ASTAR_RADIUS * ASTAR_RADIUS) - 1,
            [&](gpp::default_graph_index from, gpp::default_graph_index to) -> float {
                Position* fromPos = graph.vertex(from);
                Position* toPos = graph.vertex(to);
                return fromPos->distance(*toPos);
            },
            [](gpp::default_graph_index from, gpp::default_graph_index to, const my_edge& e) -> float {
                return e.weight;
            }
        );
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time = end - begin;

        meanTime += time.count();
    }
    meanTime /= RUN_COUNT;
    std::cout << "AStar for " << ASTAR_RADIUS << " radius took " << meanTime
              << "s on average (limit: " << frameTime
              << "s, "
              << RUN_COUNT << " runs)"
              << std::endl;
    ASSERT_LT(meanTime, frameTime);

}*/
