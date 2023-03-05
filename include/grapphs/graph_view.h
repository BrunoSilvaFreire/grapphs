#ifndef GRAPPHS_GRAPH_VIEW_H
#define GRAPPHS_GRAPH_VIEW_H
#include <vector>
#include <cstdint>
namespace gpp {


    template<typename t_graph, template<typename> typename t_iterator>
    class graph_view {
    public:
        using graph_type = t_graph;
        using index_type = typename graph_type::index_type;
        using iterator_type = t_iterator<graph_view<t_graph, t_iterator>>;
    private:
        graph_type& _graph;
        std::vector<index_type> _indices;

    public:
        graph_view(
            graph_type& graph,
            const std::vector<index_type>& indices
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
        index_type _index;
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
        index_type _index;
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