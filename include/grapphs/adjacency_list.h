
#ifndef GRAPPHS_ADJACENCY_LIST_H
#define GRAPPHS_ADJACENCY_LIST_H

#include <grapphs/graph.h>
#include <unordered_map>
#include <utility>

namespace gpp {
    template<typename V, typename E, typename GraphIndex = DefaultGraphIndex>
    class AdjacencyList : public Graph<V, E, GraphIndex> {
    public:
        AdjacencyList() = default;

        class Node {
        public:
            typedef std::unordered_map<GraphIndex, E> ConnectionMap;
        private:
            ConnectionMap map;
            V vertex;
        public:
            Node() : map(0), vertex() {}

            explicit Node(const V &data) : vertex(std::move(data)) {

            }

            const std::unordered_map<GraphIndex, E> &connections() const {
                return map;
            }

            V &data() {
                return vertex;
            }

            E *edge(GraphIndex to) {
                ConnectionMap::iterator found = map.find(to);
                if (found == map.end()) {
                    return nullptr;
                }
                return &found->second;
            }

            void connect(GraphIndex index, const E &edgeData) {
                map.emplace(index, std::move(edgeData));
            }
        };

    private:
        std::vector<Node> nodes;
    public:
        GraphIndex size() override {
            return nodes.size();
        }

        GraphIndex push(V &vertex) {
            size_t index = nodes.size();
            nodes.emplace_back(vertex);
            return static_cast<GraphIndex>(index);
        }

        V *vertex(GraphIndex index) override {
            return &node(index).data();
        }

        Node &node(GraphIndex index) {
            return nodes[index];
        }

        const Node &node(GraphIndex index) const {
            return nodes[index];
        }

        E *edge(GraphIndex from, GraphIndex to) override {
            return node(from).edge(to);
        }

        void connect(GraphIndex from, GraphIndex to, E edge) override {
            node(from).connect(to, edge);
        }

        void reserve(GraphIndex size) {
            nodes.reserve(size);
        }

        void resize(GraphIndex numVertices) {
            nodes.resize(numVertices);
        }

        std::vector<std::pair<const GraphIndex, E>> edges_from(GraphIndex index) const override {
            const Node &pNode = node(index);
            std::vector<std::pair<const GraphIndex, E>> result;
            for (const std::pair<const GraphIndex, E> &item :pNode.connections()) {
                result.push_back(item);
            }
            return result;
        }
    };
}

#endif
