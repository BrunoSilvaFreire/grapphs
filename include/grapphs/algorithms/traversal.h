#ifndef GRAPPHS_TRAVERSAL_H
#define GRAPPHS_TRAVERSAL_H

#include <functional>
#include <grapphs/graph.h>
#include <deque>
#include <set>

namespace gpp {
    template<typename graph_type>
    using vertex_explorer = std::function<
        void(
            typename graph_type::index_type index
        )
    >;
    template<typename graph_type>
    using edge_explorer = std::function<
        void(
            typename graph_type::index_type origin,
            typename graph_type::index_type destination
        )
    >;
    enum traversal_order {
        BREADTH,
        DEPTH
    };

    template<typename t_graph, traversal_order order>
    struct Traversal {
        using index_type = typename t_graph::index_type;

        static index_type next(std::deque<index_type>& open);
    };

    template<typename t_graph, traversal_order order>
    void traverse(
        const t_graph& graph,
        std::set<typename t_graph::index_type> startingPoints,
        const vertex_explorer<t_graph>& perVertex,
        const edge_explorer<t_graph>& perEdge
    ) {
        using vertex_type = typename t_graph::vertex_type;
        using index_type = typename t_graph::index_type;
        using edge_type = typename t_graph::edge_type;

        std::deque<index_type> open;
        std::set<index_type> visited;
        for (index_type item : startingPoints) {
            open.push_back(item);
        }
        while (!open.empty()) {
            index_type next = Traversal<t_graph, order>::next(open);
            if (visited.find(next) != visited.end()) {
                continue;
            }
            perVertex(next);
            visited.emplace(next);
            for (auto [neighbor, edge] : graph.edges_from(next)) {
                if (visited.find(neighbor) != visited.end()) {
                    continue;
                }
                perEdge(
                    static_cast<index_type>(next),
                    static_cast<index_type>(neighbor)
                );
                open.push_back(neighbor);
            }
        }
    }

    template<typename t_graph, traversal_order order>
    void traverse(
        const t_graph& graph,
        typename t_graph::index_type startingPoint,
        const vertex_explorer<t_graph>& perVertex,
        const edge_explorer<t_graph>& perEdge
    ) {
        traverse<t_graph, order>(
            graph,
            std::set<typename t_graph::index_type>({startingPoint}),
            perVertex,
            perEdge
        );
    }

}
#endif
