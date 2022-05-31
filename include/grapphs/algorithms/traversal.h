#ifndef GRAPPHS_TRAVERSAL_H
#define GRAPPHS_TRAVERSAL_H

#include <functional>
#include <grapphs/graph.h>
#include <deque>
#include <set>

namespace gpp {
    template<typename GraphType>
    using VertexExplorer = std::function<
        void(
            typename GraphType::IndexType index
        )
    >;
    template<typename GraphType>
    using EdgeExplorer = std::function<
        void(
            typename GraphType::IndexType origin,
            typename GraphType::IndexType destination
        )
    >;
    enum TraversalOrder {
        eBreadth,
        eDepth
    };

    template<typename TGraph, TraversalOrder Order>
    struct Traversal {
        using IndexType = typename TGraph::IndexType;

        static IndexType next(std::deque<IndexType>& open);
    };

    template<typename TGraph, TraversalOrder Order>
    void traverse(
        const TGraph& graph,
        std::set<typename TGraph::IndexType> startingPoints,
        const VertexExplorer<TGraph>& perVertex,
        const EdgeExplorer<TGraph>& perEdge
    ) {
        using VertexType = typename TGraph::VertexType;
        using IndexType = typename TGraph::IndexType;
        using EdgeType = typename TGraph::EdgeType;

        std::deque<IndexType> open;
        std::set<IndexType> visited;
        for (IndexType item : startingPoints) {
            open.push_back(item);
        }
        while (!open.empty()) {
            IndexType next = Traversal<TGraph, Order>::next(open);
            perVertex(next);
            visited.emplace(next);
            for (auto [neighbor, edge] : graph.edges_from(next)) {
                if (visited.find(neighbor) != visited.end()) {
                    continue;
                }
                perEdge(
                    static_cast<IndexType>(next),
                    static_cast<IndexType>(neighbor)
                );
                open.push_back(neighbor);
            }
        }
    }

    template<typename TGraph, TraversalOrder Order>
    void traverse(
        const TGraph& graph,
        typename TGraph::IndexType startingPoint,
        const VertexExplorer<TGraph>& perVertex,
        const EdgeExplorer<TGraph>& perEdge
    ) {
        traverse<TGraph, Order>(
            graph,
            std::set<typename TGraph::IndexType>({startingPoint}),
            perVertex,
            perEdge
        );
    }

}
#endif
