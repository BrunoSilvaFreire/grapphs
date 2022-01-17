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

void test_order(
    const std::function<void(
        const gpp::AdjacencyList<int, bool>& graph,
        size_t startingPoint,
        const gpp::VertexExplorer<gpp::AdjacencyList<int, bool>>& perVertex,
        const gpp::EdgeExplorer<gpp::AdjacencyList<int, bool>>& perEdge
    )>& traversalFunction,
    const std::vector<std::size_t>& vertexOrder,
    const std::vector<EdgeIdentifier>& edgeOrder
) {
    gpp::AdjacencyList<int, bool> graph = build_traversal_graph();
    std::size_t vertexIndex = 0;
    std::size_t edgeIndex = 0;
    std::vector<std::size_t> receivedVertices;
    std::vector<EdgeIdentifier> receivedEdges;
    traversalFunction(
        graph,
        0,
        [&](std::size_t vIndex) {
            auto expectedIndex = vertexOrder[vertexIndex];
            EXPECT_TRUE(vIndex == expectedIndex)
                            << "Vertex " << vIndex << " doesn't match " << expectedIndex
                            << " for step #" << vertexIndex;
            receivedVertices.emplace_back(vIndex);
            vertexIndex++;
        },
        [&](std::size_t from, std::size_t to) {
            auto expectedEdge = edgeOrder[edgeIndex];
            auto expectedOrigin = expectedEdge.first;
            auto expectedDestination = expectedEdge.second;
            receivedEdges.emplace_back(from, to);
            EXPECT_TRUE(from == expectedOrigin)
                            << "Edge origin " << from << " doesn't match " << expectedOrigin
                            << " for step #" << edgeIndex <<
                            " (" << expectedOrigin << "->" << expectedDestination << " vs " << from
                            << "->" << to << ")";
            EXPECT_TRUE(to == expectedDestination)
                            << "Edge destination " << from << " doesn't match "
                            << expectedDestination
                            << " for step #" << edgeIndex <<
                            " (" << expectedOrigin << "->" << expectedDestination << " vs " << from
                            << "->" << to << ")";
            edgeIndex++;
        }
    );
    GTEST_LOG_(INFO) << "Received vertices:";
    print_order(receivedVertices);
    GTEST_LOG_(INFO) << "Expected vertices:";
    print_order(vertexOrder);
    GTEST_LOG_(INFO) << "Received edges:";
    print_order(receivedEdges);
    GTEST_LOG_(INFO) << "Expected edges:";
    print_order(edgeOrder);
}

TEST(grapphs, traversal_breadth_order) {
    std::vector<std::size_t> vertexOrder = {
        0, 1, 2, 3, 6, 4, 5, 7, 8, 9
    };
    std::vector<EdgeIdentifier> edgeOrder = {
        {0, 1},
        {1, 2},
        {1, 3},
        {2, 6},
        {3, 4},
        {3, 5},
        {6, 7},
        {6, 8},
        {6, 9}
    };

    test_order(
        [](const gpp::AdjacencyList<int, bool>& graph,
           size_t startingPoint,
           const gpp::VertexExplorer<gpp::AdjacencyList<int, bool>>& perVertex,
           const gpp::EdgeExplorer<gpp::AdjacencyList<int, bool>>& perEdge) {
            gpp::breadth_first_traverse<gpp::AdjacencyList<int, bool>>(
                graph,
                startingPoint,
                perVertex,
                perEdge
            );
        },
        vertexOrder,
        edgeOrder
    );
}

TEST(grapphs, traversal_depth_order) {
    std::vector<std::size_t> vertexOrder = {
        0, 1, 3, 5, 4, 2, 6, 9, 8, 7
    };
    std::vector<EdgeIdentifier> edgeOrder = {
        {0, 1},
        {1, 2},
        {1, 3},
        {3, 4},
        {3, 5},
        {2, 6},
        {6, 7},
        {6, 8},
        {6, 9},
    };
    test_order(
        [](const gpp::AdjacencyList<int, bool>& graph,
           size_t startingPoint,
           const gpp::VertexExplorer<gpp::AdjacencyList<int, bool>>& perVertex,
           const gpp::EdgeExplorer<gpp::AdjacencyList<int, bool>>& perEdge) {
            gpp::depth_first_traverse<gpp::AdjacencyList<int, bool>>(
                graph,
                startingPoint,
                perVertex,
                perEdge
            );
        },
        vertexOrder,
        edgeOrder
    );
}