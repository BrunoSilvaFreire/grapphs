#ifndef GRAPPHS_RLO_TRAVERSAL_H
#define GRAPPHS_RLO_TRAVERSAL_H

#include <grapphs/algorithms/bfs_traversal.h>

namespace gpp {

    template<typename graph_type>
    void reverse_level_order_traverse(
        const graph_type& graph,
        typename graph_type::index_type startingPoint,
        const vertex_explorer<graph_type>& perVertex,
        const edge_explorer<graph_type>& perEdge
    ) {
        using index_type = typename graph_type::index_type;

        std::vector<index_type> verticesOrder;
        std::vector<std::tuple<index_type, index_type>> edgesOrder;

        gpp::breadth_first_traverse(
            graph,
            startingPoint,
            [&](index_type vertex) {
                verticesOrder.emplace_back(vertex);
            },
            [&](index_type from, index_type to) {
                edgesOrder.emplace_back(from, to);
            }
        );

        std::reverse(verticesOrder.begin(), verticesOrder.end());
        std::reverse(edgesOrder.begin(), edgesOrder.end());

        for (index_type vertex : verticesOrder) {
            perVertex(vertex);
            for (auto [from, to] : edgesOrder) {
                if (to == vertex) {
                    perEdge(from, to);
                }
            }
        }
    }

    template<typename graph_type>
    void reverse_level_order_traverse(
        const graph_type& graph,
        const std::set<typename graph_type::index_type>& startingPoints,
        const vertex_explorer<graph_type>& perVertex,
        const edge_explorer<graph_type>& perEdge
    ) {
        using index_type = typename graph_type::index_type;

        std::vector<index_type> verticesOrder;
        std::vector<std::tuple<index_type, index_type>> edgesOrder;

        gpp::breadth_first_traverse(
            graph,
            startingPoints,
            [&](index_type vertex) {
                verticesOrder.emplace_back(vertex);
            },
            [&](index_type from, index_type to) {
                edgesOrder.emplace_back(from, to);
            }
        );

        std::reverse(verticesOrder.begin(), verticesOrder.end());
        std::reverse(edgesOrder.begin(), edgesOrder.end());

        for (index_type vertex : verticesOrder) {
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