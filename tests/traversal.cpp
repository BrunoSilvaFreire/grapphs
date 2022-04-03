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
#include "grapphs/tests/traversal_order.h"

typedef std::pair<std::size_t, std::size_t> EdgeIdentifier;

void print_order(const std::vector<size_t>& order) {
    std::stringstream vertices;
    for (int i = 0; i < order.size(); ++i) {
        vertices << order[i];
        if (i != order.size() - 1) {
            vertices << ", ";
        }
    }
    GTEST_LOG_(INFO) << vertices.str();
}

void print_order(const std::vector<std::pair<size_t, size_t>>& order) {
    std::stringstream vertices;
    for (int i = 0; i < order.size(); ++i) {
        const std::pair<size_t, size_t>& pair = order[i];
        vertices << pair.first << "->" << pair.second;
        if (i != order.size() - 1) {
            vertices << ", ";
        }
    }
    GTEST_LOG_(INFO) << vertices.str();
}

gpp::AdjacencyList<int, bool> build_traversal_graph() {
    gpp::AdjacencyList<int, bool> graph;
    for (int i = 0; i <= 9; ++i) {
        graph.push(0);
    }
    graph.connect(0, 1, true);
    graph.connect(1, 2, true);
    graph.connect(1, 3, true);
    graph.connect(2, 6, true);
    graph.connect(3, 4, true);
    graph.connect(3, 5, true);
    graph.connect(6, 7, true);
    graph.connect(6, 8, true);
    graph.connect(6, 9, true);
    return graph;
}

template<gpp::TraversalOrder Order>
void test_order(
    gpp::ExpectedTraversalOrder& expected
) {
    gpp::AdjacencyList<int, bool> graph = build_traversal_graph();
    std::size_t vertexIndex = 0;
    std::size_t edgeIndex = 0;
    std::vector<std::size_t> receivedVertices;
    std::vector<EdgeIdentifier> receivedEdges;
    gpp::traverse<gpp::AdjacencyList<int, bool>, Order>(
        graph,
        0,
        [&](std::size_t vertex) {
            bool isCorrect = expected.pop(vertex);
            EXPECT_TRUE(isCorrect) << "Vertex " << vertex << " was not allowed for step #" << vertexIndex
                                   << "(" << expected.listAvailable() << ")";
            receivedVertices.emplace_back(vertex);
            vertexIndex++;
        },
        [&](std::size_t from, std::size_t to) {
/*            auto expectedEdge = edgeOrder[edgeIndex];
            auto expectedOrigin = expectedEdge.first;
            auto expectedDestination = expectedEdge.second;*/
            receivedEdges.emplace_back(from, to);
            /*  EXPECT_TRUE(from == expectedOrigin)
                              << "Edge origin " << from << " doesn't match " << expectedOrigin
                              << " for step #" << edgeIndex <<
                              " (" << expectedOrigin << "->" << expectedDestination << " vs " << from
                              << "->" << to << ")";
              EXPECT_TRUE(to == expectedDestination)
                              << "Edge destination " << from << " doesn't match "
                              << expectedDestination
                              << " for step #" << edgeIndex <<
                              " (" << expectedOrigin << "->" << expectedDestination << " vs " << from
                              << "->" << to << ")";*/
            edgeIndex++;
        }
    );
    GTEST_LOG_(INFO) << "Received vertices:";
    print_order(receivedVertices);
    GTEST_LOG_(INFO) << "Received edges:";
    print_order(receivedEdges);
}

TEST(grapphs, traversal_breadth_order) {
    gpp::ExpectedTraversalOrder order = {
        0
    };
    order.then({1})
        ->then({2, 3})
        ->then({4, 5, 6})
        ->then({7, 8, 9});

    test_order<gpp::TraversalOrder::eBreadth>(order);
}

TEST(grapphs, traversal_depth_order) {
    gpp::ExpectedTraversalOrder order = {
        0
    };
    order.then({1})
        ->then({2})
        ->then({6})
        ->then({7, 8, 9})
        ->then({3})
        ->then({4, 5});

    test_order<gpp::TraversalOrder::eDepth>(order);
}