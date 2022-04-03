#ifndef UNNECESSARYENGINE_REVERSE_LEVEL_ORDER_H
#define UNNECESSARYENGINE_REVERSE_LEVEL_ORDER_H

#include <grapphs/algorithms/bfs_traversal.h>

namespace gpp {

    template<typename TGraph>
    void reverse_level_order_traverse(
        const TGraph& graph,
        typename TGraph::IndexType startingPoint,
        const VertexExplorer<TGraph>& perVertex,
        const EdgeExplorer<TGraph>& perEdge
    ) {
        using IndexType = typename TGraph::IndexType;
        using VertexType = typename TGraph::VertexType;
        using EdgeType = typename TGraph::EdgeType;
        std::vector<IndexType> verticesOrder;
        std::vector<std::tuple<IndexType, IndexType>> edgesOrder;
        gpp::breadth_first_traverse(
            graph,
            startingPoint,
            [&](IndexType vertex) {
                verticesOrder.emplace_back(vertex);
            },
            [&](IndexType from, IndexType to) {
                edgesOrder.emplace_back(from, to);
            }
        );
        std::reverse(verticesOrder.begin(), verticesOrder.end());
        std::reverse(edgesOrder.begin(), edgesOrder.end());
        for (IndexType vertex : verticesOrder) {
            perVertex(vertex);
            for (auto [from, to] : edgesOrder) {
                if (to == vertex) {
                    perEdge(from, to);
                }
            }
        }
    }
}
#endif