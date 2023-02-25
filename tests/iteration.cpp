#include <gtest/gtest.h>
#include <grapphs/tests/mazes.h>

TEST(grapphs, graph_iteration) {
    gpp::adjacency_list<int, float> graph;

    std::vector<int> entries = {5, 10, 15, 20, 25, 30, 50, 100};

    std::set<int> pending(entries.begin(), entries.end());

    for (int value : entries) {
        graph.push(value);
    }

    for (int* vertex : graph) {
        auto numErased = pending.erase(*vertex);

        EXPECT_GT(numErased, 0) << "Vertex " << *vertex
                                << " was either already removed or doesn't exist in the graph.";
    }

    EXPECT_TRUE(pending.empty())
                    << "Not all vertices were iterated on. (" << pending.size() << " pending)";
}

TEST(grapphs, edges_from) {
    gpp::adjacency_list<int, float> graph;

    auto first = graph.push(5);
    auto second = graph.push(10);
    auto third = graph.push(15);
    graph.connect(first, second, 0.5F);
    graph.connect(second, third, 1.0F);
    graph.connect(first, third, 2.0F);

    using index_type = gpp::adjacency_list<int, float>::index_type;
    std::set<std::pair<index_type, float>> pending = {
        {second, 0.5F},
        {third, 2.0F}
    };

    for (const auto& [destination, edge] : graph.edges_from(first)) {
        unsigned long numErased = pending.erase({destination, edge});

        EXPECT_GT(numErased, 0) << "Unable to delete connection from vertex " << first << " to "
                                << destination
                                << " with edge of value " << edge << ".";
    }

    EXPECT_TRUE(pending.empty()) << "Not all connections were iterated on. ("
    << pending.size() << " pending)";

}