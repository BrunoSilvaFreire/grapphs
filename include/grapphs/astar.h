//
// Created by bruno on 05/04/2021.
//

#ifndef GRAPPHS_ASTAR_H
#define GRAPPHS_ASTAR_H

#include <grapphs/graph.h>
#include <queue>
#include <limits>

namespace gpp {
    template<typename I>
    class LessByFScore {
    private:
        std::unordered_map<I, float> *map;

        float try_find(I index) {
            std::unordered_map<I, float>::iterator it = map->find(index);
            if (it == map->end()) {
                return std::numeric_limits<float>::max();
            }
            return it->second;
        }

    public:

        bool operator()(I a, I b) {
            float aScore = try_find(a);
            float bScore = try_find(b);
            return aScore > bScore;
        }
    };

    template<typename V, typename E, typename G, typename I = DefaultGraphIndex>
    void astar(
            const G &graph,
            I from,
            I to,
            std::function<float(I from, I to)> heuristics
    ) {
        std::unordered_map<I, float> gScore;
        std::unordered_map<I, float> fScore;
        std::unordered_map<I, I> history;
        std::priority_queue<I, std::vector<I>, LessByFScore<I>> open((gpp::LessByFScore<I>(&fScore)));

        gScore[from] = 0;
        fScore[from] = heuristics(from, to);
        open.push(from);

        while (!open.empty()) {
            I next = open.top();
            open.pop();
            if (next == to) {
                //return retrace_path(history, next);
            }
            for (const auto[neighborIndex, edge] : graph.edges_from(next)) {
            }
        }
    }
}
#endif //GRAPPHS_ASTAR_H
