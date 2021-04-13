//
// Created by bruno on 04/04/2021.
//

#ifndef GRAPPHS_ADJACENCY_MATRIX_H
#define GRAPPHS_ADJACENCY_MATRIX_H

#include <grapphs/graph.h>

namespace gpp {
    template<typename V, typename E, typename GraphIndex = DefaultGraphIndex>
    class AdjacencyMatrix : public Graph<V, E, GraphIndex> {
    private:
        std::vector<V> vertices;
        std::vector<E> edges;
    public:
        explicit AdjacencyMatrix(GraphIndex size) : vertices(size), edges(size * size) {
        }

        template<typename D>
        explicit AdjacencyMatrix(Graph <V, E, D> *other) : vertices(), edges() {
            auto size = static_cast<GraphIndex>(other->size());
            vertices.resize(size);
            edges.resize(size * size);
            for (GraphIndex i = 0; i < size; ++i) {
                V vertex;
                if (other->try_get_vertex(i, vertex)) {
                    vertices[i] = vertex;
                }
            }
            for (GraphIndex x = 0; x < size; ++x) {
                for (GraphIndex y = 0; y < size; ++y) {
                    E edge;
                    if (other->try_get_edge(x, y, edge)) {
                        GraphIndex i = index(x, y);
                        edges[i] = edge;
                    }
                }
            }
        }

        GraphIndex size() override {
            return vertices.size();
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
    };

}
#endif //GRAPPHS_ADJACENCY_MATRIX_H
