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
    class less_by_f_score {
    private:
        std::unordered_map<I, float>* _map;

        float try_find(I index) {
            typename std::unordered_map<I, float>::iterator it = _map->find(index);
            if (it == _map->end()) {
                return std::numeric_limits<float>::max();
            }
            return it->second;
        }

    public:
        explicit less_by_f_score(std::unordered_map<I, float>* map) : _map(map) {}

        bool operator()(I a, I b) {
            float aScore = try_find(a);
            float bScore = try_find(b);
            return aScore > bScore;
        }
    };

    template<typename T>
    class priority_queue : public std::priority_queue<T, std::vector<T>, less_by_f_score<T>> {
    public:
        explicit priority_queue(
            const less_by_f_score<T>& pred
        ) : std::priority_queue<T, std::vector<T>, less_by_f_score<T>>(pred) {}

        bool contains(const T& item) {
            for (const T& other : this->c) {
                if (other == item) {
                    return true;
                }
            }
            return false;
        }
    };

    template<typename t_index>
    class graph_path {
    public:
        using index_type = t_index;

    private:
        std::vector<index_type> _vertices;

    public:
        graph_path() : _vertices() {}

        explicit graph_path(const std::vector<index_type>& vertices) : _vertices(vertices) {}

        const std::vector<index_type>& get_vertices() const {
            return _vertices;
        }

        size_t count() const {
            return _vertices.size();
        }

        void for_each(const std::function<void(index_type from, index_type to)>& block) {
            for (int i = 0; i < _vertices.size() - 1; ++i) {
                block(_vertices[i], _vertices[i + 1]);
            }
        }
    };
    template<typename t_graph>
    using heuristics_function = std::function<float(typename t_graph::index_type from, typename t_graph::index_type to)>;

    template<typename t_graph>
    using distance_function = std::function<
        float(
            typename t_graph::index_type from,
            typename t_graph::index_type to,
            const typename t_graph::edge_type& edge)
    >;

    template<typename t_graph>

#ifdef __cpp_concepts
    requires gpp::is_graph<t_graph>
#endif
    graph_path<typename t_graph::index_type> astar(
        t_graph& graph,
        typename t_graph::index_type from,
        typename t_graph::index_type to,
        heuristics_function<t_graph> heuristics,
        distance_function<t_graph> distanceFunction
    )
    {
        using index_type = typename t_graph::index_type;
        using iterator = typename std::unordered_map<index_type, float>::iterator;

        std::unordered_map<index_type, float> gScore;
        std::unordered_map<index_type, float> fScore;
        std::unordered_map<index_type, index_type> history;

        gpp::less_by_f_score<index_type> predicate(&fScore);
        priority_queue<index_type> open(predicate);

        gScore[from] = 0;
        fScore[from] = heuristics(from, to);
        open.push(from);

        while (!open.empty()) {
            index_type next = open.top();
            open.pop();
            if (next == to) {
                std::vector<index_type> indices;
                index_type current = to;
                indices.push_back(current);
                do {
                    auto candidate = history.find(current);
                    if (candidate == history.end()) {

                        throw std::runtime_error("");
                    }
                    current = (*candidate).second;
                    indices.push_back(current);
                }
                while (current != from);
                std::reverse(indices.begin(), indices.end());
                return graph_path<index_type>(indices);
            }
            for (const auto [neighborIndex, edge] : graph.edges_from(next)) {
                float attempt = gScore[next] + distanceFunction(next, neighborIndex, edge);
                iterator existing = gScore.find(neighborIndex);

                // Not yet explored or we found a better path?
                if (existing == gScore.end() || (*existing).second > attempt) {
                    history[neighborIndex] = next;
                    gScore[neighborIndex] = attempt;
                    fScore[neighborIndex] = attempt + distanceFunction(neighborIndex, to, edge);
                    if (!open.contains(neighborIndex)) {
                        open.push(neighborIndex);
                    }
                }

            }
        }
        throw std::runtime_error("Unable to find path");
    }
}
#endif //GRAPPHS_ASTAR_H
