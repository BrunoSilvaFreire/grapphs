//
// Created by bruno on 05/04/2021.
//

#ifndef GRAPPHS_ASTAR_H
#define GRAPPHS_ASTAR_H

#include <grapphs/graph.h>
#include <stdexcept>
#include <queue>
#include <limits>

namespace gpp {
    template<typename I>
    class LessByFScore {
    private:
        std::unordered_map<I, float> *map;

        float try_find(I index) {
            typename std::unordered_map<I, float>::iterator it = map->find(index);
            if (it == map->end()) {
                return std::numeric_limits<float>::max();
            }
            return it->second;
        }

    public:
        explicit LessByFScore(std::unordered_map<I, float> *map) : map(map) {}

        bool operator()(I a, I b) {
            float aScore = try_find(a);
            float bScore = try_find(b);
            return aScore > bScore;
        }
    };

    template<typename T>
    class PriorityQueue : public std::priority_queue<T, std::vector<T>, LessByFScore<T>> {
    public:
        explicit PriorityQueue(
                const LessByFScore<T> &pred
        ) : std::priority_queue<T, std::vector<T>, LessByFScore<T>>(pred) {}

        bool contains(const T &item) {
            for (const T &other : this->c) {
                if (other == item) {
                    return true;
                }
            }
            return false;
        }
    };

    template<typename IndexType>
    class GraphPath {
    public:
    private:
        std::vector<IndexType> vertices;
    public:
        GraphPath() : vertices() {}

        GraphPath(const std::vector<IndexType> &vertices) : vertices(vertices) {}

        size_t count() const {
            return vertices.size();
        }
    };

    template<typename G>
    GraphPath<typename G::IndexType> astar(
            G &graph,
            typename G::IndexType from,
            typename G::IndexType to,
            std::function<float(typename G::IndexType from, typename G::IndexType to)> heuristics,
            std::function<float(typename G::IndexType from, typename G::IndexType to,
                                const typename G::EdgeType &edge)> distanceCalculator
    )
//#ifdef __cpp_concepts
//    requires std::is_assignable_v<gpp::Graph<typename GraphType::VertexType, typename GraphType::EdgeType, typename GraphType::IndexType>;
//#endif
    {
        using IndexType = typename G::IndexType;
        using Iterator = typename std::unordered_map<IndexType, float>::iterator;
        std::unordered_map<IndexType, float> gScore;
        std::unordered_map<IndexType, float> fScore;
        std::unordered_map<IndexType, IndexType> history;
        gpp::LessByFScore<IndexType> predicate(&fScore);
        PriorityQueue<IndexType> open(predicate);

        gScore[from] = 0;
        fScore[from] = heuristics(from, to);
        open.push(from);

        while (!open.empty()) {
            IndexType next = open.top();
            open.pop();
            if (next == to) {
                std::vector<IndexType> indices;
                IndexType current = to;
                indices.push_back(current);
                do {
                    auto candidate = history.find(current);
                    if (candidate == history.end()) {

                        throw std::runtime_error("");
                    }
                    current = (*candidate).second;
                    indices.push_back(current);
                } while (current != from);
                std::reverse(indices.begin(), indices.end());
                return GraphPath<IndexType>(indices);
            }
            for (const auto[neighborIndex, edge] : graph.edges_from(next)) {
                float attempt = gScore[next] + distanceCalculator(next, neighborIndex, edge);
                Iterator existing = gScore.find(neighborIndex);
                // Not yet explored or we found a better path?
                if (existing == gScore.end() || (*existing).second > attempt) {
                    history[neighborIndex] = next;
                    gScore[neighborIndex] = attempt;
                    fScore[neighborIndex] = attempt + distanceCalculator(neighborIndex, to, edge);
                    if (!open.contains(neighborIndex)) {
                        open.push(neighborIndex);
                    }
                }
            }
        }
    }
}
#endif //GRAPPHS_ASTAR_H
