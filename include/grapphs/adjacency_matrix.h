//
// Created by bruno on 04/04/2021.
//

#ifndef GRAPPHS_ADJACENCY_MATRIX_H
#define GRAPPHS_ADJACENCY_MATRIX_H

#include <grapphs/graph.h>
#include <stdexcept>

namespace gpp {
    template<typename V, typename E, typename I = DefaultGraphIndex>
    class AdjacencyMatrix : public Graph<AdjacencyMatrix<V, E, I>> {
    public:
        typedef typename GraphTraits<AdjacencyMatrix<V, E, I>>::VertexType VertexType;
        typedef typename GraphTraits<AdjacencyMatrix<V, E, I>>::EdgeType EdgeType;
        typedef typename GraphTraits<AdjacencyMatrix<V, E, I>>::IndexType IndexType;
    private:
        std::vector<VertexType> vertices;
        std::vector<EdgeType> edges;
    public:

        explicit AdjacencyMatrix(IndexType size) : vertices(size), edges(size * size) {
        }

        template<typename G>
        explicit AdjacencyMatrix(Graph <G> *other) : vertices(), edges() {
            auto size = static_cast<IndexType>(other->size());
            vertices.resize(size);
            edges.resize(size * size);
            for (IndexType i = 0; i < size; ++i) {
                VertexType vertex;
                if (other->try_get_vertex(i, vertex)) {
                    vertices[i] = vertex;
                }
            }
            for (IndexType x = 0; x < size; ++x) {
                for (IndexType y = 0; y < size; ++y) {
                    EdgeType edge;
                    if (other->try_get_edge(x, y, edge)) {
                        IndexType i = index(x, y);
                        edges[i] = edge;
                    }
                }
            }
        }

        IndexType size() {
            return vertices.size();
        }

        VertexType *vertex(IndexType index) {
            return static_cast<VertexType *>(&vertices[index]);
        }

        IndexType index(IndexType from, IndexType to) {
            return from * vertices.size() + to;
        }

        EdgeType *edge(IndexType from, IndexType to) {
            return &edges[index(from, to)];
        }

        void connect(IndexType from, IndexType to, EdgeType edge) {
            edges[index(from, to)] = edge;
        }

        std::vector<std::pair<const IndexType, EdgeType>> edges_from(IndexType vertex) const {
            throw std::runtime_error("Not implemented yet");
        }
    };

    template<typename V, typename E, typename I>
    struct GraphTraits<AdjacencyMatrix<V, E, I>> {
        typedef V VertexType;
        typedef E EdgeType;
        typedef I IndexType;

        static IndexType call_size(AdjacencyMatrix<V, E, I> *implementation) {
            return implementation->size();
        }

        static VertexType *call_vertex(AdjacencyMatrix<V, E, I> *implementation, IndexType index) {
            return implementation->vertex(index);
        }

        static EdgeType *call_edge(AdjacencyMatrix<V, E, I> *implementation, IndexType from, IndexType to) {
            return implementation->edge(from, to);
        }

        static void call_connect(AdjacencyMatrix<V, E, I> *implementation, IndexType from, IndexType to, EdgeType edge) {
            return implementation->connect(from, to, edge);
        }

        static auto call_edges_from(AdjacencyMatrix<V, E, I> *implementation, IndexType index) {
            return implementation->edges_from(index);
        }
    };

}
#endif //GRAPPHS_ADJACENCY_MATRIX_H
