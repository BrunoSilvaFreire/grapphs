
#ifndef GRAPPHS_ADJACENCY_LIST_H
#define GRAPPHS_ADJACENCY_LIST_H

#include <grapphs/graph.h>
#include <unordered_map>
#include <utility>
#include <queue>

namespace gpp {

    template<
        typename VertexType,
        typename EdgeType,
        typename IndexType = DefaultGraphIndex
    >
    class AdjacencyList : public Graph<VertexType, EdgeType, IndexType> {
    public:
        AdjacencyList() = default;

        class Node {
        public:
            typedef std::unordered_map<IndexType, EdgeType> ConnectionMap;
        private:
            ConnectionMap map;
            VertexType vertex;
        public:
            Node() : map(0), vertex() {}

            explicit Node(const VertexType& data) : vertex(std::move(data)) {

            }

            const ConnectionMap& connections() const {
                return map;
            }

            ConnectionMap& connections() {
                return map;
            }

            VertexType& data() {
                return vertex;
            }

            const VertexType& data() const {
                return vertex;
            }

            EdgeType* edge(IndexType to) {
                typename ConnectionMap::iterator found = map.find(to);
                if (found == map.end()) {
                    return nullptr;
                }
                return &found->second;
            }

            void connect(IndexType index, const EdgeType& edgeData) {
                map.emplace(index, std::move(edgeData));
            }

            bool disconnect(IndexType index) {
                return map.erase(index) > 0;
            }

            void clear() {
                map.clear();
            }
        };

        class EdgeView {
        public:
            typedef std::vector<std::pair<IndexType, EdgeType>> ConnectionVector;
        private:
            ConnectionVector elements;
        public:
            typedef typename ConnectionVector::iterator Iterator;
            typedef typename ConnectionVector::const_iterator ConstIterator;

            explicit EdgeView(const Node& owner) {
                auto& connections = owner.connections();
                size_t num = connections.size();
                elements.resize(num);
                size_t i = 0;
                for (const std::pair<IndexType, EdgeType>& item : connections) {
                    elements[i++] = item;
                }
            }

            Iterator begin() {
                return elements.begin();
            }

            Iterator end() {
                return elements.end();
            }

            ConstIterator begin() const { return elements.begin(); }

            ConstIterator end() const { return elements.end(); }
        };

        void clear() {
            nodes.clear();
        }

    private:
        std::vector<Node> nodes;
        std::queue<IndexType> freeIndices;
    public:
        void remove(const IndexType& index) {
            // std::memset(&nodes[index], 0, sizeof(VertexType));
            nodes[index].clear();
            freeIndices.push(index);
        }

        IndexType push(const VertexType& vertex) {
            IndexType index;
            if (!freeIndices.empty()) {
                index = freeIndices.front();
                nodes[index].data() = vertex;
                freeIndices.pop();
            } else {
                index = static_cast<IndexType>(nodes.size());
                nodes.emplace_back(vertex);
            }
            return index;
        }

        IndexType push(VertexType&& vertex) {
            IndexType index;
            if (!freeIndices.empty()) {
                index = freeIndices.front();
                nodes[index].data() = vertex;
            } else {
                index = static_cast<IndexType>(nodes.size());
                nodes.emplace_back(std::move(vertex));
            }
            return index;
        }

        IndexType size() const {
            return nodes.size();
        }

        VertexType* vertex(IndexType index) {
            return &node(index).data();
        }

        const VertexType* vertex(IndexType index) const override {
            return &node(index).data();
        }

        EdgeType* edge(IndexType from, IndexType to) {
            return node(from).edge(to);
        }

        void connect(IndexType from, IndexType to, EdgeType edge) {
            node(from).connect(to, edge);
        }

        EdgeView edges_from(IndexType index) {
            return EdgeView(node(index));
        }

        EdgeView edges_from(IndexType index) const {
            return EdgeView(node(index));
        }

        Node& node(IndexType index) {
            return nodes[index];
        }

        const Node& node(IndexType index) const {
            return nodes[index];
        }

        void reserve(IndexType size) {
            nodes.reserve(size);
        }

        void resize(IndexType numVertices) {
            nodes.resize(numVertices);
        }

        bool disconnect(IndexType from, IndexType to) override {
            return node(from).disconnect(to);
        }

        typedef typename Graph<VertexType, EdgeType, IndexType>::GraphIterator GraphIterator;
        typedef typename Graph<VertexType, EdgeType, IndexType>::ConstGraphIterator ConstGraphIterator;

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

        class PairedGraphIterator
            : public std::iterator<std::input_iterator_tag, IndexType> {
        public:
            bool operator==(const PairedGraphIterator& rhs) const { return i == rhs.i; }

            bool operator!=(const PairedGraphIterator& rhs) const { return i != rhs.i; }

            std::pair<const VertexType*, IndexType> operator*() const {
                return std::make_pair(owner->vertex(i), i);
            }

            PairedGraphIterator& operator++() {
                i++;
                return *this;
            }

            typedef Graph<VertexType, EdgeType, IndexType> OwnerGraph;
        public:
            PairedGraphIterator(const OwnerGraph* owner, IndexType i) : owner(owner),
                                                                        i(i) {}

        protected:
            friend OwnerGraph;


            const OwnerGraph* owner;
            IndexType i;
        };

        typedef typename Graph<VertexType, EdgeType, IndexType>::template GraphView<PairedGraphIterator> PairedGraphView;

        PairedGraphView all_vertices() {
            return PairedGraphView(this);
        };
    };
}

#endif
