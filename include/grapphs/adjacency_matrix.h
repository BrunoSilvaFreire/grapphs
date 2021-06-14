//
// Created by bruno on 04/04/2021.
//

#ifndef GRAPPHS_ADJACENCY_MATRIX_H
#define GRAPPHS_ADJACENCY_MATRIX_H

#include <grapphs/graph.h>
#include <stdexcept>

namespace gpp {
    template<typename VertexType, typename EdgeType, typename IndexType = DefaultGraphIndex>
    class AdjacencyMatrix : public Graph<VertexType, EdgeType, IndexType> {
    private:
        std::vector<VertexType> vertices;
        std::vector<EdgeType> edges;
    public:

        explicit AdjacencyMatrix(IndexType size) : vertices(size), edges(size * size) {
        }

        explicit AdjacencyMatrix(Graph <VertexType, EdgeType, IndexType> *other) : vertices(), edges() {
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

        IndexType size() const {
            return vertices.size();
        }

        VertexType *vertex(IndexType index) {
            return static_cast<VertexType *>(&vertices[index]);
        }

        const VertexType *vertex(IndexType index) const {
            return static_cast<const VertexType *>(&vertices[index]);
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

        bool disconnect(IndexType from, IndexType to) override {
            edges[index(from, to)] = EdgeType();
        }

        typedef typename Graph<VertexType, EdgeType, IndexType>::GraphIterator GraphIterator;
        typedef typename Graph<VertexType, EdgeType, IndexType>::ConstGraphIterator ConstGraphIterator;
        friend GraphIterator;

        GraphIterator begin() override {
            return GraphIterator(this, 0);
        }

        GraphIterator end() override {
            return GraphIterator(this, size());
        }

        ConstGraphIterator begin() const override {
            return ConstGraphIterator(this, 0);
        }

        ConstGraphIterator end() const override {
            return ConstGraphIterator(this, size());
        }
    };

}
#endif //GRAPPHS_ADJACENCY_MATRIX_H
