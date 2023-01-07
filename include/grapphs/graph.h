#ifndef GRAPPHS_GRAPH_H
#define GRAPPHS_GRAPH_H

#include <unordered_map>
#include <functional>
#include <iterator>

#if __cpp_concepts

#include <concepts>

#endif

namespace gpp {
    typedef size_t default_graph_index;

    template<typename t_vertex, typename t_edge, typename t_index = default_graph_index>
    class graph {
    public:

        using vertex_type = t_vertex;
        using edge_type = t_edge;
        using index_type = t_index;

        /**
         * @returns The number of vertices in this graph.
         */
        virtual index_type size() const = 0;

        /**
         * @returns A pointer to the vertex corresponding to the given index, or nullptr is none is found.
         */
        virtual vertex_type* vertex(index_type index) = 0;

        /**
         * @returns A const pointer to the vertex corresponding to the given index,
         * or nullptr is none is found.
         */
        virtual const vertex_type* vertex(index_type index) const = 0;

        /**
         * @returns A pointer to the edge connecting the vertex corresponding to the 'from' index,
         * to the vertex corresponding to the 'to' index, or nullptr is none is found.
         */
        virtual edge_type* edge(index_type from, index_type to) = 0;

        /**
         * Connects the given vertices using a one way connection represented by the given
         * \p edge
         */
        virtual void connect(index_type from, index_type to, edge_type edge) = 0;

        /**
         * Disconnects the given vertices removing the previous edge that was present.
         */
        virtual bool disconnect(index_type from, index_type to) = 0;

        bool try_get_vertex(index_type index, vertex_type& output) {
            vertex_type* ptr = vertex(index);
            if (ptr != nullptr) {
                output = *ptr;
            }
            return ptr != nullptr;
        }

        bool try_get_edge(index_type index, index_type to, edge_type& output) {
            edge_type* ptr = edge(index, to);
            if (ptr != nullptr) {
                output = *ptr;
            }
            return ptr != nullptr;
        }

    private:
        using owner_graph = graph<vertex_type, edge_type, index_type>;
    public:
        template<class t_iterator>
        class graph_view {
        public:
            using iterator = t_iterator;
        private:
            iterator _first, _last;
        public:
            explicit graph_view(
                owner_graph* graph
            ) : _first(graph, 0),
                _last(graph, graph->size()) {

            }

            explicit graph_view(
                const owner_graph* graph
            ) : _first(graph, 0),
                _last(graph, graph->size()) {

            }

            iterator begin() {
                return _first;
            }

            iterator end() {
                return _last;
            }
        };

        class graph_iterator {
        public:
            typedef std::input_iterator_tag iterator_category;
            typedef index_type value_type;
            typedef std::ptrdiff_t difference_type;
            typedef index_type* pointer;
            typedef index_type& reference;

            bool operator==(const graph_iterator& rhs) const { return _index == rhs._index; }

            bool operator!=(const graph_iterator& rhs) const { return _index != rhs._index; }

            vertex_type* operator*() {
                return _owner->vertex(_index);
            }

            graph_iterator& operator++() {
                _index++;
                return *this;
            }

        public:
            graph_iterator(owner_graph* owner, index_type i) : _owner(owner), _index(i) {}

        protected:

            friend owner_graph;

            owner_graph* _owner;
            index_type _index;

        };

        class const_graph_iterator {
        public:
            typedef std::input_iterator_tag iterator_category;
            typedef index_type value_type;
            typedef std::ptrdiff_t difference_type;
            typedef index_type* pointer;
            typedef index_type& reference;

            bool operator==(const const_graph_iterator& rhs) const { return _index == rhs._index; }

            bool operator!=(const const_graph_iterator& rhs) const { return _index != rhs._index; }

            const vertex_type* operator*() const {
                return _owner->vertex(_index);
            }

            graph_iterator& operator++() {
                _index++;
                return *this;
            }

        public:
            const_graph_iterator(
                const owner_graph* owner,
                index_type i
            ) : _owner(owner),
                _index(i) {}

        protected:
            const owner_graph* _owner;
            index_type _index;
        };

        virtual graph_iterator begin() = 0;

        virtual graph_iterator end() = 0;

        virtual const_graph_iterator begin() const = 0;

        virtual const_graph_iterator end() const = 0;
    };

#if __cpp_concepts

    template<typename graph_type>
    concept is_graph = std::is_base_of_v<
        gpp::_graph<
            typename graph_type::vertex_type,
            typename graph_type::edgeType,
            typename graph_type::index_type
        >,
        graph_type
    >;

#endif

}
#endif
