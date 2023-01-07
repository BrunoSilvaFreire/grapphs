#ifndef GRAPPHS_BFS_TRAVERSAL_H
#define GRAPPHS_BFS_TRAVERSAL_H

#include <grapphs/algorithms/traversal.h>

namespace gpp {
    template<typename t_graph>
    struct Traversal<t_graph, traversal_order::BREADTH> {
        using index_type = typename t_graph::index_type;

        static index_type next(std::deque<index_type>& open) {
            index_type value = open.front();
            open.pop_front();
            return value;
        }
    };

    template<typename t_graph>
    void breadth_first_traverse(
        const t_graph& graph,
        std::set<typename t_graph::index_type> startingPoints,
        const vertex_explorer<t_graph>& perVertex,
        const edge_explorer<t_graph>& perEdge
    ) {
        traverse<t_graph, traversal_order::BREADTH>(graph, startingPoints, perVertex, perEdge);
    }

    template<typename t_graph>
    void breadth_first_traverse(
        const t_graph& graph,
        typename t_graph::index_type startingPoint,
        const vertex_explorer<t_graph>& perVertex,
        const edge_explorer<t_graph>& perEdge
    ) {
        traverse<t_graph, traversal_order::BREADTH>(graph, startingPoint, perVertex, perEdge);
    }

}
#endif