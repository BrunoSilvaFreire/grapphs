
#ifndef GRAPPHS_ADJACENCY_LIST_H
#define GRAPPHS_ADJACENCY_LIST_H

#include <grapphs/graph.h>
#include <unordered_map>
#include <utility>

namespace gpp {
    template<typename V, typename E, typename I = DefaultGraphIndex>
    class AdjacencyList : public Graph<AdjacencyList<V, E, I>> {
    public:
        typedef typename GraphTraits<AdjacencyList<V, E, I>>::VertexType VertexType;
        typedef typename GraphTraits<AdjacencyList<V, E, I>>::EdgeType EdgeType;
        typedef typename GraphTraits<AdjacencyList<V, E, I>>::IndexType IndexType;

        AdjacencyList() = default;

        class Node {
        public:
            typedef std::unordered_map<IndexType, EdgeType> ConnectionMap;
        private:
            ConnectionMap map;
            VertexType vertex;
        public:
            Node() : map(0), vertex() {}

            explicit Node(const VertexType &data) : vertex(std::move(data)) {

            }

            const std::unordered_map<IndexType, EdgeType> &connections() const {
                return map;
            }

            std::unordered_map<IndexType, EdgeType> &connections() {
                return map;
            }

            VertexType &data() {
                return vertex;
            }

            EdgeType *edge(IndexType to) {
                typename ConnectionMap::iterator found = map.find(to);
                if (found == map.end()) {
                    return nullptr;
                }
                return &found->second;
            }

            void connect(IndexType index, const EdgeType &edgeData) {
                map.emplace(index, std::move(edgeData));
            }
        };

        class EdgeView {
        public:
            typedef typename Node::ConnectionMap::iterator Iterator;
            typedef typename Node::ConnectionMap::const_iterator ConstIterator;

            explicit EdgeView(Node& owner) : owner(owner) {}

            Iterator begin() { return owner.connections().begin(); }
            Iterator end() { return owner.connections().end();}

            ConstIterator begin() const { return owner.connections().begin(); }
            ConstIterator end() const { return owner.connections().end();}
        private:
            Node& owner;
        };

    private:
        std::vector<Node> nodes;
    public:


        IndexType push(const VertexType& vertex){
            size_t index = nodes.size();
            nodes.emplace_back(vertex);
            return static_cast<IndexType>(index);
        }

        IndexType push(VertexType&& vertex){
            size_t index = nodes.size();
            nodes.emplace_back(std::move(vertex));
            return static_cast<IndexType>(index);
        }

        IndexType size() {
            return nodes.size();
        }

        VertexType *vertex(IndexType index)  {
            return &node(index).data();
        }

        EdgeType *edge(IndexType from, IndexType to) {
            return node(from).edge(to);
        }

        void connect(IndexType from, IndexType to, EdgeType edge) {
            node(from).connect(to, edge);
        }

        EdgeView edges_from(IndexType index) {
            return EdgeView(node(index));
        }

        Node &node(IndexType index) {
            return nodes[index];
        }

        const Node &node(IndexType index) const {
            return nodes[index];
        }

        void reserve(IndexType size) {
            nodes.reserve(size);
        }

        void resize(IndexType numVertices) {
            nodes.resize(numVertices);
        }
    };

    template<typename V, typename E, typename I>
    struct GraphTraits<AdjacencyList<V, E, I>> {
        typedef V VertexType;
        typedef E EdgeType;
        typedef I IndexType;

        static IndexType call_size(AdjacencyList<V, E, I>* implementation){
            return implementation->size();
        }

        static VertexType* call_vertex(AdjacencyList<V, E, I>* implementation, IndexType index){
            return implementation->vertex(index);
        }

        static EdgeType* call_edge(AdjacencyList<V, E, I>* implementation, IndexType from, IndexType to){
            return implementation->edge(from, to);
        }

        static void call_connect(AdjacencyList<V, E, I>* implementation, IndexType from, IndexType to, EdgeType edge){
            return implementation->connect(from, to, edge);
        }

        static auto call_edges_from(AdjacencyList<V, E, I>* implementation, IndexType index) {
            return implementation->edges_from(index);
        }
    };
}

#endif
