#include <gtest/gtest.h>
#include <grapphs/adjacency_list.h>

TEST(grapphs, removal) {
    gpp::adjacency_list<int, float> graph;

    graph.push(0);
    auto toBeRemoved = graph.push(1);
    graph.push(2);

    EXPECT_EQ(graph.size(), 3);
    graph.remove(toBeRemoved);
    EXPECT_EQ(graph.size(), 2);

    int numIterations = 0;
    std::set<int> expected = { 0, 2 };
    for (const auto [index, vertex] : graph.all_vertices()) {
        EXPECT_NE(*vertex, 1);
        auto numRemoved = expected.erase(*vertex);
        EXPECT_GT(numRemoved, 0)<< "Vertex " << *vertex
                                << " was not found in pending set";
        numIterations++;
    }
    EXPECT_EQ(numIterations, 2);
}