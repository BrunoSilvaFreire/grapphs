#include <gtest/gtest.h>
#include <grapphs/adjacency_matrix.h>

constexpr int k_graph_size = 64;

TEST(grapphs, adjacency_matrix_disconnecting) {
    gpp::adjacency_matrix<int, int> graph(k_graph_size);

    for (std::size_t i = 0; i < k_graph_size; ++i) {
        graph[i] = static_cast<int>(i);
    }

    for (std::size_t i = 0; i < k_graph_size; ++i) {
        for (std::size_t j = 0; j < k_graph_size; ++j) {
            graph.connect(i, j, i * j);
        }
    }

    int from = k_graph_size / 2;
    int to = k_graph_size;
    graph.disconnect(from, to);
    EXPECT_EQ(*graph.edge(from, to), 0);
}