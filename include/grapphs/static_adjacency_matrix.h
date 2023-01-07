//
// Created by bruno on 05/04/2021.
//

#ifndef GRAPPHS_STATIC_ADJACENCY_MATRIX_H
#define GRAPPHS_STATIC_ADJACENCY_MATRIX_H

#include <grapphs/graph.h>
#include <array>
#include <stdexcept>

namespace gpp {
    template<typename V, typename E, size_t Size, typename GraphIndex = default_graph_index>
    class static_adjacency_matrix : public graph<V, E, GraphIndex> {
    private:
        std::array<V, Size> _vertices;
        std::array<E, Size * Size> _edges;
    public:
        static_adjacency_matrix() : _vertices(), _edges() {

        }

        template<typename D>
        explicit static_adjacency_matrix(graph<V, E, D>* other) : static_adjacency_matrix() {
            for (GraphIndex i = 0; i < Size; ++i) {
                V vertex;
                if (other->try_get_vertex(i, vertex)) {
                    _vertices[i] = vertex;
                }
            }
            for (GraphIndex x = 0; x < Size; ++x) {
                for (GraphIndex y = 0; y < Size; ++y) {
                    E edge;
                    if (other->try_get_edge(x, y, edge)) {
                        GraphIndex i = index(x, y);
                        _edges[i] = edge;
                    }
                }
            }
        }

        GraphIndex size() override {
            return Size;
        }

        V* vertex(GraphIndex index) override {
            return static_cast<V*>(&_vertices[index]);
        }

        GraphIndex index(GraphIndex from, GraphIndex to) {
            return from * _vertices.size() + to;
        }

        E* edge(GraphIndex from, GraphIndex to) override {
            return &_edges[index(from, to)];
        }

        void connect(GraphIndex from, GraphIndex to, E edge) override {
            _edges[index(from, to)] = edge;
        }

        std::vector<std::pair<const GraphIndex, E>> edges_from(GraphIndex vertex) const override {
            throw std::runtime_error("Not implemented yet");
        }
    };
}
#endif //GRAPPHS_STATIC_ADJACENCY_MATRIX_H
