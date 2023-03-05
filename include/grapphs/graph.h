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
    };

#if __cpp_concepts

    template<typename graph_type>
    concept is_graph = std::is_base_of_v<
        gpp::graph<
            typename graph_type::vertex_type,
            typename graph_type::edge_type,
            typename graph_type::index_type
        >,
        graph_type
    >;

#endif

    template<typename t_graph, template<typename> typename t_iterator>
    class graph_view {
    public:
        using graph_type = t_graph;
        using index_type = typename graph_type::index_type;
        using iterator_type = t_iterator<graph_view<t_graph, t_iterator>>;
    private:
        graph_type& _graph;
        std::vector<std::size_t> _indices;

    public:
        graph_view(
            graph_type& graph,
            const std::vector<std::size_t>& indices
        ) : _graph(graph), _indices(indices) {}

        iterator_type begin() {
            return iterator_type(*this, 0);
        }

        iterator_type end() {
            return iterator_type(*this, _indices.size());
        }

        iterator_type begin() const {
            return iterator_type(*this, 0);
        }

        iterator_type end() const {
            return iterator_type(*this, _indices.size());
        }

        template<typename>
        friend class vertex_iterator;

        template<typename>
        friend class const_vertex_iterator;
    };

    template<typename t_view>
    class vertex_iterator {
    private:
        using graph_type = typename t_view::graph_type;
        using vertex_type = typename graph_type::vertex_type;
        using index_type = typename graph_type::index_type;

        t_view& _owner;
        std::size_t _index;
    public:
        vertex_iterator(t_view& owner, size_t index) : _owner(owner), _index(index) {}

        void operator++() {
            _index++;
        }
        bool operator==(const vertex_iterator& other) {
            return _index == other._index;
        }

        bool operator!=(const vertex_iterator& other) {
            return !this->operator==(other);
        }

        std::pair<index_type, vertex_type*> operator*() {
            index_type vertexIndex = _owner._indices[_index];
            vertex_type* vertex = _owner._graph.vertex(vertexIndex);
            return std::pair<index_type, vertex_type*>(vertexIndex, vertex);
        }
    };

    template<typename t_view>
    class const_vertex_iterator {
    private:
        using graph_type = typename t_view::graph_type;
        using vertex_type = typename graph_type::vertex_type;
        using index_type = typename graph_type::index_type;

        t_view& _owner;
        std::size_t _index;
    public:
        const_vertex_iterator(t_view& owner, size_t index) : _owner(owner), _index(index) {}

        void operator++() {
            _index++;
        }
        bool operator==(const const_vertex_iterator& other) {
            return _index == other._index;
        }

        bool operator!=(const const_vertex_iterator& other) {
            return !this->operator==(other);
        }

        std::pair<index_type, const vertex_type*> operator*() {
            index_type vertexIndex = _owner._indices[_index];
            const vertex_type* vertex = _owner._graph.vertex(vertexIndex);
            return std::pair<index_type, const vertex_type*>(vertexIndex, vertex);
        }
    };

}
#endif
