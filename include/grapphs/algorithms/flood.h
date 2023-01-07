#ifndef GRAPPHS_FLOOD_H
#define GRAPPHS_FLOOD_H

#include <set>

#include <grapphs/graph.h>
#include <grapphs/algorithms/bfs_traversal.h>
#include <queue>

namespace gpp {

#if __cpp_concepts

    template<
        gpp::is_graph graph_type,
        typename StartPointsContainer
    >
#else

    template<
        typename graph_type,
        typename StartPointsContainer = std::set<typename graph_type::index_type>
    >
#endif
    void flood(
        const graph_type& graph,
        const StartPointsContainer& startingPoints,
        const vertex_explorer<graph_type>& perVertex,
        const edge_explorer<graph_type>& perEdge
    )
#if __cpp_concepts
    requires std::input_iterator<typename StartPointsContainer::iterator>
#endif
    {
        gpp::breadth_first_traverse(graph, startingPoints, perVertex, perEdge);
    }
}
#endif //GRAPPHS_FLOOD_H
