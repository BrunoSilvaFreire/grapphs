#ifndef GRAPPHS_FLOOD_H
#define GRAPPHS_FLOOD_H

#include <set>

#include <grapphs/graph.h>
#include <grapphs/algorithms/bfs_traversal.h>
#include <queue>

namespace gpp {

#if __cpp_concepts

    template<
        gpp::is_graph GraphType,
        typename StartPointsContainer
    >
#else

    template<
        typename GraphType,
        typename StartPointsContainer = std::set<typename GraphType::IndexType>
    >
#endif
    void flood(
        const GraphType& graph,
        const StartPointsContainer& startingPoints,
        const VertexExplorer<GraphType>& perVertex,
        const EdgeExplorer<GraphType>& perEdge
    )
#if __cpp_concepts
    requires std::input_iterator<typename StartPointsContainer::iterator>
#endif
    {
        gpp::breadth_first_traverse(graph, startingPoints, perVertex, perEdge);
    }
}
#endif //GRAPPHS_FLOOD_H
