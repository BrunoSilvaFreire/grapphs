#ifndef GRAPPHS_GRAPH_H
#define GRAPPHS_GRAPH_H

#include <unordered_map>
#include <functional>
#include <iterator>

namespace gpp {
    typedef size_t DefaultGraphIndex;

    template<typename I>
    struct GraphTraits;

    template<typename Implementation>
    class Graph {
    public:
        typedef typename GraphTraits<Implementation>::VertexType VertexType;
        typedef typename GraphTraits<Implementation>::EdgeType EdgeType;
        typedef typename GraphTraits<Implementation>::IndexType IndexType;

        inline Implementation* derived_ptr() { return static_cast<Implementation*>(this); }

        IndexType size() {
            return GraphTraits<Implementation>::call_size(derived_ptr());
        }

        VertexType* vertex(IndexType index) {
            return GraphTraits<Implementation>::call_vertex(derived_ptr(), index);
        }

        EdgeType* edge(IndexType from, IndexType to) {
            return GraphTraits<Implementation>::call_edge(derived_ptr(), from, to);
        }

        void connect(IndexType from, IndexType to, EdgeType edge) {
            GraphTraits<Implementation>::call_connect(derived_ptr(), from, to, edge);
        }

        auto edges_from(IndexType index) {
            return GraphTraits<Implementation>::call_edges_from(derived_ptr(), index);
        }

        bool try_get_vertex(IndexType index, VertexType &output) {
            VertexType *ptr = vertex(index);
            if (ptr != nullptr) {
                output = *ptr;
            }
            return ptr != nullptr;
        }

        bool try_get_edge(IndexType index, IndexType to, EdgeType &output) {
            EdgeType *ptr = edge(index, to);
            if (ptr != nullptr) {
                output = *ptr;
            }
            return ptr != nullptr;
        }

    public:

        class GraphIterator : public std::iterator<std::input_iterator_tag, IndexType> {
        public:
            bool operator==(const GraphIterator& rhs) const { return i == rhs.i; }

            bool operator!=(const GraphIterator& rhs) const { return i != rhs.i; }

            VertexType* operator*() {
                return owner->vertex(i);
            }
            GraphIterator& operator++() {
                i++;
                return *this;
            }

        protected:
            friend Graph<Implementation>;
            GraphIterator(Graph<Implementation>* owner, IndexType i) : owner(owner), i(i) {}

            Graph<Implementation>* owner;
            IndexType i;

        };

        GraphIterator begin() { return GraphIterator(this, 0); }
        GraphIterator end() { return GraphIterator(this, GraphTraits<Implementation>::call_size(derived_ptr())); }

        GraphIterator begin() const { return GraphIterator(this, 0); }
        GraphIterator end() const { return GraphIterator(this, GraphTraits<Implementation>::call_size(derived_ptr())); }
    };
}
#endif
