#ifndef UNNECESSARYENGINE_BFS_TRAVERSAL_H
#define UNNECESSARYENGINE_BFS_TRAVERSAL_H

#include <grapphs/algorithms/traversal.h>

namespace gpp {
    template<typename TGraph>
    struct Traversal<TGraph, TraversalOrder::eBreadth> {
        using IndexType = typename TGraph::IndexType;

        static IndexType next(std::deque<IndexType>& open) {
            IndexType value = open.front();
            open.pop_front();
            return value;
        }
    };

    template<typename TGraph>
    void breadth_first_traverse(
        const TGraph& graph,
        std::set<typename TGraph::IndexType> startingPoints,
        const VertexExplorer<TGraph>& perVertex,
        const EdgeExplorer<TGraph>& perEdge
    ) {
        traverse<TGraph, TraversalOrder::eBreadth>(graph, startingPoints, perVertex, perEdge);
    }

    template<typename TGraph>
    void breadth_first_traverse(
        const TGraph& graph,
        typename TGraph::IndexType startingPoint,
        const VertexExplorer<TGraph>& perVertex,
        const EdgeExplorer<TGraph>& perEdge
    ) {
        traverse<TGraph, TraversalOrder::eBreadth>(graph, startingPoint, perVertex, perEdge);
    }

}
#endif