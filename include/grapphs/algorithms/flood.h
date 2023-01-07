#ifndef GRAPPHS_FLOOD_H
#define GRAPPHS_FLOOD_H

#include <set>

#include <grapphs/graph.h>
#include <grapphs/algorithms/bfs_traversal.h>
#include <queue>

namespace gpp {

    template<
        typename graph_type,
        typename origin_points_container= std::set<typename graph_type::index_type>
    >
#if __cpp_concepts
    requires gpp::is_graph<graph_type>
#endif
    void flood(
        const graph_type& graph,
        const origin_points_container& startingPoints,
        const vertex_explorer<graph_type>& perVertex,
        const edge_explorer<graph_type>& perEdge
    )
#if __cpp_concepts
    requires std::input_iterator<typename origin_points_container::iterator>
#endif
    {
        gpp::breadth_first_traverse(graph, startingPoints, perVertex, perEdge);
    }
}
#endif //GRAPPHS_FLOOD_H
