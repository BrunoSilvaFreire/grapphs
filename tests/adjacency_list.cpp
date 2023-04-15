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

TEST(grapphs, disconnecting) {
    gpp::adjacency_list<int, int> graph;

    auto fromDisconnect = graph.push(0);
    graph.push(1);
    auto toDisconnect = graph.push(2);

    graph.connect(0, 1, 100);
    graph.connect(1, 2, 200);
    graph.connect(0, 2, 300);

    EXPECT_EQ(graph.size(), 3);
    ASSERT_NE(graph.edge(fromDisconnect, toDisconnect), nullptr);
    EXPECT_EQ(*graph.edge(fromDisconnect, toDisconnect), 300);
    EXPECT_TRUE(graph.disconnect(fromDisconnect, toDisconnect));
    EXPECT_EQ(graph.edge(fromDisconnect, toDisconnect), nullptr);
    EXPECT_FALSE(graph.disconnect(fromDisconnect, toDisconnect));

    {
        int numIterations = 0;
        std::set<int> expected = { 100 };
        for (const auto [index, edge] : graph.edges_from(fromDisconnect)) {
            auto numRemoved = expected.erase(edge);
            EXPECT_GT(numRemoved, 0) << "Edge with value " << edge
                                    << " wasn't in expected set";
            numIterations++;
        }
        EXPECT_EQ(numIterations, 1);
        EXPECT_TRUE(expected.empty());
    }
}