#ifndef GRAPPHS_GRAPH_H
#define GRAPPHS_GRAPH_H

#include <unordered_map>
#include <functional>
#include <iterator>

#if __cpp_concepts

#include <concepts>

#endif

namespace gpp {
    typedef size_t DefaultGraphIndex;

    template<
        typename TVertex,
        typename TEdge,
        typename TIndex = DefaultGraphIndex
    >
    class Graph {
    public:
        typedef TVertex VertexType;
        typedef TEdge EdgeType;
        typedef TIndex IndexType;

        virtual IndexType size() const = 0;

        virtual VertexType* vertex(IndexType index) = 0;

        virtual const VertexType* vertex(IndexType index) const = 0;

        virtual EdgeType* edge(IndexType from, IndexType to) = 0;

        virtual void connect(IndexType from, IndexType to, EdgeType edge) = 0;

        virtual bool disconnect(IndexType from, IndexType to) = 0;

        bool try_get_vertex(IndexType index, VertexType& output) {
            VertexType* ptr = vertex(index);
            if (ptr != nullptr) {
                output = *ptr;
            }
            return ptr != nullptr;
        }

        bool try_get_edge(IndexType index, IndexType to, EdgeType& output) {
            EdgeType* ptr = edge(index, to);
            if (ptr != nullptr) {
                output = *ptr;
            }
            return ptr != nullptr;
        }

    private:
        typedef Graph<VertexType, EdgeType, IndexType> OwnerGraph;
    public:
        template<class IteratorType>
        class GraphView {
        private:
            IteratorType first, last;
        public:
            explicit GraphView(const OwnerGraph* graph) : first(graph, 0),
                                                          last(graph, graph->size()) {

            }

            IteratorType& begin() {
                return first;
            }

            IteratorType& end() {
                return last;
            }
        };

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

            typedef Graph<VertexType, EdgeType, IndexType> OwnerGraph;
        public:
            GraphIterator(OwnerGraph* owner, IndexType i) : owner(owner), i(i) {}

        protected:

            friend OwnerGraph;


            OwnerGraph* owner;
            IndexType i;

        };

        class ConstGraphIterator
            : public std::iterator<std::input_iterator_tag, IndexType> {
        public:
            bool operator==(const ConstGraphIterator& rhs) const { return i == rhs.i; }

            bool operator!=(const ConstGraphIterator& rhs) const { return i != rhs.i; }

            const VertexType* operator*() const {
                return owner->vertex(i);
            }

            GraphIterator& operator++() {
                i++;
                return *this;
            }

            typedef Graph<VertexType, EdgeType, IndexType> OwnerGraph;
        public:
            ConstGraphIterator(const OwnerGraph* owner, IndexType i) : owner(owner),
                                                                       i(i) {}

        protected:

            friend OwnerGraph;


            const OwnerGraph* owner;
            IndexType i;

        };


        virtual GraphIterator begin() = 0;

        virtual GraphIterator end() = 0;

        virtual ConstGraphIterator begin() const = 0;

        virtual ConstGraphIterator end() const = 0;
    };

#if __cpp_concepts

    template<typename GraphType>
    concept is_graph = std::is_base_of_v<
        gpp::Graph<
            typename GraphType::VertexType,
            typename GraphType::EdgeType,
            typename GraphType::IndexType
        >,
        GraphType
    >;

#endif

}
#endif
