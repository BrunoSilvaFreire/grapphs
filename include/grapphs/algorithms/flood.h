#ifndef GRAPPHS_FLOOD_H
#define GRAPPHS_FLOOD_H

#include <set>
#include <queue>
#include <iterator>
#include <grapphs/graph.h>
#include <grapphs/algorithms/bfs_traversal.h>

#if __cpp_concepts
#include <concepts>
#endif

namespace gpp {

    template<
        typename graph_type,
        typename origin_points_container= std::set<typename graph_type::index_type>
    >
#if __cpp_concepts
    requires gpp::is_graph<graph_type>
    &&
    std::input_iterator<typename origin_points_container::iterator>
#endif
    void flood(
        const graph_type& graph,
        const origin_points_container& startingPoints,
        const vertex_explorer<graph_type>& perVertex,
        const edge_explorer<graph_type>& perEdge
    ) {
        gpp::breadth_first_traverse(graph, startingPoints, perVertex, perEdge);
    }
}
#endif //GRAPPHS_FLOOD_H
