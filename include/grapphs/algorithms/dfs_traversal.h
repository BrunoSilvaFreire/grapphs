#ifndef UNNECESSARYENGINE_DFS_TRAVERSAL_H
#define UNNECESSARYENGINE_DFS_TRAVERSAL_H

#include <grapphs/algorithms/traversal.h>

namespace gpp {
    template<typename TGraph>
    struct Traversal<TGraph, TraversalOrder::eDepth> {
        using IndexType = typename TGraph::IndexType;

        static IndexType next(std::deque<IndexType>& open) {
            IndexType value = open.back();
            open.pop_back();
            return value;
        }
    };


    template<typename TGraph>
    void depth_first_traverse(
        const TGraph& graph,
        std::set<typename TGraph::IndexType> startingPoints,
        const VertexExplorer<TGraph>& perVertex,
        const EdgeExplorer<TGraph>& perEdge
    ) {
        traverse<TGraph, TraversalOrder::eDepth>(graph, startingPoints, perVertex, perEdge);
    }

    template<typename TGraph>
    void depth_first_traverse(
        const TGraph& graph,
        typename TGraph::IndexType startingPoint,
        const VertexExplorer<TGraph>& perVertex,
        const EdgeExplorer<TGraph>& perEdge
    ) {
        traverse<TGraph, TraversalOrder::eDepth>(graph, startingPoint, perVertex, perEdge);
    }
};

#endif
