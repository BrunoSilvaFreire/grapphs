#ifndef GRAPPHS_GRAPH_H
#define GRAPPHS_GRAPH_H

#include <unordered_map>
#include <functional>
#include <iterator>

namespace gpp {
    typedef size_t DefaultGraphIndex;

    template<
            typename V,
            typename E,
            typename GraphIndex = DefaultGraphIndex
    >
    class Graph {
    public:
        typedef V VertexType;
        typedef E EdgeType;
        typedef GraphIndex IndexType;

        virtual V *vertex(GraphIndex index) = 0;

        virtual E *edge(GraphIndex from, GraphIndex to) = 0;

        virtual GraphIndex size() = 0;

        virtual void connect(GraphIndex from, GraphIndex to, E edge) = 0;

        bool try_get_vertex(GraphIndex index, V &output) {
            V *ptr = vertex(index);
            if (ptr != nullptr) {
                output = *ptr;
            }
            return ptr != nullptr;
        }

        bool try_get_edge(GraphIndex index, GraphIndex to, E &output) {
            E *ptr = edge(index, to);
            if (ptr != nullptr) {
                output = *ptr;
            }
            return ptr != nullptr;
        }

        class GraphIterator : public std::iterator<std::input_iterator_tag, GraphIndex> {
        public:

            bool operator==(const GraphIterator& rhs) const { return i == rhs.i; }

            bool operator!=(const GraphIterator& rhs) const { return i != rhs.i; }

            V& operator*() {
                return owner->vertex(i);
            }

            GraphIterator& operator++() {
                i++;
                return *this;
            }

        protected:
            GraphIterator(Graph<V, E>* owner, GraphIndex i) : owner(owner), i(i) {}

        protected:
            Graph<V, E>* owner;
            GraphIndex i;
        };


        GraphIterator begin() { return GraphIterator(this, 0); }
        GraphIterator end() { return GraphIterator(this, size()); }

        GraphIterator begin() const { return GraphIterator(this, 0); }
        GraphIterator end() const { return GraphIterator(this, size()); }


        // TODO: return an iterable instead of a newly allocated vector
        virtual std::vector<std::pair<const GraphIndex, E>> edges_from(GraphIndex vertex) const = 0;
    };


}
#endif
