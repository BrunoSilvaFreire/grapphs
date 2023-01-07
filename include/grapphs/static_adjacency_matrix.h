//
// Created by bruno on 05/04/2021.
//

#ifndef GRAPPHS_STATIC_ADJACENCY_MATRIX_H
#define GRAPPHS_STATIC_ADJACENCY_MATRIX_H

#include <grapphs/graph.h>
#include <array>
#include <stdexcept>

namespace gpp {
    template<typename t_vertex, typename t_edge, size_t Size, typename t_index = default_graph_index>
    class static_adjacency_matrix : public graph<t_vertex, t_edge, t_index> {
    public:

        using vertex_type = typename graph<t_vertex, t_edge, t_index>::vertex_type;
        using edge_type = typename graph<t_vertex, t_edge, t_index>::edge_type;
        using index_type = typename graph<t_vertex, t_edge, t_index>::index_type;

    private:
        std::array<vertex_type, Size> _vertices;
        std::array<t_edge, Size * Size> _edges;

    public:
        static_adjacency_matrix() : _vertices(), _edges() {

        }

        template<typename D>
        explicit static_adjacency_matrix(graph<vertex_type, t_edge, D>* other) : static_adjacency_matrix() {
            for (index_type i = 0; i < Size; ++i) {
                vertex_type vertex;
                if (other->try_get_vertex(i, vertex)) {
                    _vertices[i] = vertex;
                }
            }
            for (index_type x = 0; x < Size; ++x) {
                for (index_type y = 0; y < Size; ++y) {
                    t_edge edge;
                    if (other->try_get_edge(x, y, edge)) {
                        index_type i = index(x, y);
                        _edges[i] = edge;
                    }
                }
            }
        }

        index_type size() override {
            return Size;
        }

        vertex_type* vertex(index_type index) override {
            return static_cast<vertex_type*>(&_vertices[index]);
        }

        index_type index(index_type from, index_type to) {
            return from * _vertices.size() + to;
        }

        t_edge* edge(index_type from, index_type to) override {
            return &_edges[index(from, to)];
        }

        void connect(index_type from, index_type to, t_edge edge) override {
            _edges[index(from, to)] = edge;
        }

        std::vector<std::pair<const index_type, t_edge>> edges_from(index_type vertex) const override {
            throw std::runtime_error("Not implemented yet");
        }
    };
}
#endif //GRAPPHS_STATIC_ADJACENCY_MATRIX_H
