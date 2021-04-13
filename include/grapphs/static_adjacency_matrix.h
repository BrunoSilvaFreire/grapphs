//
// Created by bruno on 05/04/2021.
//

#ifndef GRAPPHS_STATIC_ADJACENCY_MATRIX_H
#define GRAPPHS_STATIC_ADJACENCY_MATRIX_H

#include <grapphs/graph.h>
#include <array>
#include <stdexcept>

namespace gpp {
    template<typename V, typename E, size_t Size, typename GraphIndex = DefaultGraphIndex>
    class StaticAdjacencyMatrix : public Graph<V, E, GraphIndex> {
    private:
        std::array<V, Size> vertices;
        std::array<E, Size * Size> edges;
    public:
        StaticAdjacencyMatrix() : vertices(), edges() {

        }

        template<typename D>
        explicit StaticAdjacencyMatrix(Graph <V, E, D> *other) : StaticAdjacencyMatrix() {
            for (GraphIndex i = 0; i < Size; ++i) {
                V vertex;
                if (other->try_get_vertex(i, vertex)) {
                    vertices[i] = vertex;
                }
            }
            for (GraphIndex x = 0; x < Size; ++x) {
                for (GraphIndex y = 0; y < Size; ++y) {
                    E edge;
                    if (other->try_get_edge(x, y, edge)) {
                        GraphIndex i = index(x, y);
                        edges[i] = edge;
                    }
                }
            }
        }

        GraphIndex size() override {
            return Size;
        }

        V *vertex(GraphIndex index) override {
            return static_cast<V *>(&vertices[index]);
        }

        GraphIndex index(GraphIndex from, GraphIndex to) {
            return from * vertices.size() + to;
        }

        E *edge(GraphIndex from, GraphIndex to) override {
            return &edges[index(from, to)];
        }

        void connect(GraphIndex from, GraphIndex to, E edge) override {
            edges[index(from, to)] = edge;
        }

        std::vector<std::pair<const GraphIndex, E>> edges_from(GraphIndex vertex) const override {
            throw std::runtime_error("Not implemented yet");
        }
    };
}
#endif //GRAPPHS_STATIC_ADJACENCY_MATRIX_H
