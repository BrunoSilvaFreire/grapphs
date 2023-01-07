
#ifndef GRAPPHS_ADJACENCY_LIST_H
#define GRAPPHS_ADJACENCY_LIST_H

#include <grapphs/graph.h>
#include <unordered_map>
#include <utility>
#include <queue>

namespace gpp {

    template<
        typename t_vertex,
        typename t_edge,
        typename t_index = default_graph_index
    >
    class adjacency_list : public graph<t_vertex, t_edge, t_index> {
    public:

        using vertex_type = typename graph<t_vertex, t_edge, t_index>::vertex_type;
        using edge_type = typename graph<t_vertex, t_edge, t_index>::edge_type;
        using index_type = typename graph<t_vertex, t_edge, t_index>::index_type;

        adjacency_list() = default;

        class adjacency_node {
        public:
            typedef std::unordered_map<index_type, edge_type> connection_map;
        private:
            connection_map _map;
            vertex_type _vertex;
        public:
            adjacency_node() : _map(0), _vertex() {}

            explicit adjacency_node(const vertex_type& data) : _vertex(data) {

            }

            explicit adjacency_node(vertex_type&& data) : _vertex(std::move(data)) {

            }

            const connection_map& connections() const {
                return _map;
            }

            connection_map& connections() {
                return _map;
            }

            vertex_type& data() {
                return _vertex;
            }

            const vertex_type& data() const {
                return _vertex;
            }

            edge_type* edge(index_type to) {
                typename connection_map::iterator found = _map.find(to);
                if (found == _map.end()) {
                    return nullptr;
                }
                return &found->second;
            }

            const edge_type* edge(index_type to) const {
                typename connection_map::const_iterator found = _map.find(to);
                if (found == _map.end()) {
                    return nullptr;
                }
                return &found->second;
            }

            void connect(index_type index, const edge_type& edgeData) {
                _map.emplace(index, std::move(edgeData));
            }

            bool disconnect(index_type index) {
                return _map.erase(index) > 0;
            }

            void clear() {
                _map.clear();
            }
        };

        class edge_view {
        public:
            typedef std::vector<std::pair<index_type, edge_type>> connection_vector;
        private:
            connection_vector _elements;
        public:
            typedef typename connection_vector::iterator iterator;
            typedef typename connection_vector::const_iterator const_iterator;

            explicit edge_view(const adjacency_node& owner) {
                auto& connections = owner.connections();
                size_t num = connections.size();
                _elements.resize(num);
                size_t i = 0;
                for (const std::pair<index_type, edge_type>& item : connections) {
                    _elements[i++] = item;
                }
            }

            iterator begin() {
                return _elements.begin();
            }

            iterator end() {
                return _elements.end();
            }

            const_iterator begin() const { return _elements.begin(); }

            const_iterator end() const { return _elements.end(); }
        };

        void clear() {
            _nodes.clear();
        }

    private:
        std::vector<adjacency_node> _nodes;
        std::queue<index_type> _freeIndices;
    public:
        void remove(const index_type& index) {
            // std::memset(&nodes[index], 0, sizeof(vertex_type));
            _nodes[index].clear();
            _freeIndices.push(index);
        }

        index_type push(const vertex_type& vertex) {
            index_type index;
            if (!_freeIndices.empty()) {
                index = _freeIndices.front();
                _nodes[index].data() = vertex;
                _freeIndices.pop();
            }
            else {
                index = static_cast<index_type>(_nodes.size());
                _nodes.emplace_back(vertex);
            }
            return index;
        }

        index_type push(vertex_type&& vertex) {
            index_type index;
            if (!_freeIndices.empty()) {
                index = _freeIndices.front();
                _freeIndices.pop();
                _nodes[index].data() = std::move(vertex);
            }
            else {
                index = static_cast<index_type>(_nodes.size());
                _nodes.emplace_back(std::move(vertex));
            }
            return index;
        }

        index_type size() const final {
            return static_cast<index_type>(_nodes.size());
        }

        vertex_type* vertex(index_type index) final {
            return &node(index).data();
        }

        const vertex_type* vertex(index_type index) const final {
            return &node(index).data();
        }

        edge_type* edge(index_type from, index_type to) final {
            return node(from).edge(to);
        }

        const edge_type* edge(index_type from, index_type to) const {
            return node(from).edge(to);
        }

        void connect(index_type from, index_type to, edge_type edge) final {
            node(from).connect(to, edge);
        }

        edge_view edges_from(index_type index) {
            return edge_view(node(index));
        }

        edge_view edges_from(index_type index) const {
            return edge_view(node(index));
        }

        adjacency_node& node(index_type index) {
            return _nodes[index];
        }

        const adjacency_node& node(index_type index) const {
            return _nodes[index];
        }

        void reserve(index_type size) {
            _nodes.reserve(size);
        }

        void resize(index_type numVertices) {
            _nodes.resize(numVertices);
        }

        bool disconnect(index_type from, index_type to) final {
            return node(from).disconnect(to);
        }

        typedef typename graph<vertex_type, edge_type, index_type>::graph_iterator graph_iterator;
        typedef typename graph<
            vertex_type,
            edge_type,
            index_type
        >::const_graph_iterator const_graph_iterator;

        graph_iterator begin() final {
            return graph_iterator(this, 0);
        }

        graph_iterator end() final {
            return graph_iterator(this, size());
        }

        const_graph_iterator begin() const final {
            return const_graph_iterator(this, 0);
        }

        const_graph_iterator end() const final {
            return const_graph_iterator(this, size());
        }

        class paired_graph_iterator {
        public:
            typedef std::input_iterator_tag iterator_category;
            typedef std::pair<vertex_type*, index_type> value_type;
            typedef std::ptrdiff_t difference_type;
            typedef value_type* pointer;
            typedef value_type& reference;

            bool operator==(const paired_graph_iterator& rhs) const { return _index == rhs._index; }

            bool operator!=(const paired_graph_iterator& rhs) const { return _index != rhs._index; }

            std::pair<const vertex_type*, index_type> operator*() const {
                return std::make_pair(_owner->vertex(_index), _index);
            }

            std::pair<vertex_type*, index_type> operator*() {
                return std::make_pair(_owner->vertex(_index), _index);
            }

            paired_graph_iterator& operator++() {
                _index++;
                return *this;
            }

            typedef graph<vertex_type, edge_type, index_type> owner_graph;
        public:
            paired_graph_iterator(owner_graph* owner, index_type i) : _owner(owner),
                                                                      _index(i) {}

        protected:
            friend owner_graph;

            owner_graph* _owner;
            index_type _index;
        };

        class const_paired_graph_iterator {
        public:
            typedef std::input_iterator_tag iterator_category;
            typedef std::pair<const vertex_type*, index_type> value_type;
            typedef std::ptrdiff_t difference_type;
            typedef value_type* pointer;
            typedef value_type& reference;

            bool operator==(const const_paired_graph_iterator& rhs) const {
                return _index == rhs._index;
            }

            bool operator!=(const const_paired_graph_iterator& rhs) const {
                return _index != rhs._index;
            }

            std::pair<const vertex_type*, index_type> operator*() const {
                return std::make_pair(_owner->vertex(_index), _index);
            }

            const_paired_graph_iterator& operator++() {
                _index++;
                return *this;
            }

            typedef graph<vertex_type, edge_type, index_type> owner_graph;
        public:
            const_paired_graph_iterator(
                const owner_graph* owner,
                index_type i
            ) : _owner(owner), _index(i) {}

        protected:
            friend owner_graph;
            const owner_graph* _owner;
            index_type _index;
        };

        typedef typename graph<vertex_type, edge_type, index_type>::template graph_view<
            const_paired_graph_iterator
        > const_paired_graph_view;
        typedef typename graph<vertex_type, edge_type, index_type>::template graph_view<
            paired_graph_iterator
        > paired_graph_view;

        const_paired_graph_view all_vertices() const {
            return const_paired_graph_view(this);
        };

        paired_graph_view all_vertices() {
            return paired_graph_view(this);
        };
    };
}

#endif
