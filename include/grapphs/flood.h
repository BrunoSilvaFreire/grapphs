#ifndef GRAPPHS_FLOOD_H
#define GRAPPHS_FLOOD_H

#include <set>

#include <grapphs/graph.h>
#include <queue>

namespace gpp {
    template<typename GraphType>
    using GraphExplorer = std::function<
        void(
            typename GraphType::IndexType origin,
            typename GraphType::IndexType index,
            const typename GraphType::VertexType& vertex,
            const typename GraphType::EdgeType& edge
        )
    >;
#if __cpp_concepts

    template<
        gpp::is_graph GraphType,
        typename StartPointsContainer
    >
#else
    template<
        typename GraphType,
        typename StartPointsContainer
    >
#endif
    void flood(
        const GraphType& graph,
        const StartPointsContainer& startingPoints,
        const GraphExplorer<GraphType>& onDiscovered
    )
#if __cpp_concepts
    requires std::input_iterator<typename StartPointsContainer::iterator>
#endif
    {
        using VertexType = typename GraphType::VertexType;
        using IndexType = typename GraphType::IndexType;
        using EdgeType = typename GraphType::EdgeType;
        std::queue<IndexType> open;
        std::set<IndexType> visited;
        for (IndexType item : startingPoints) {
            open.push(item);
        }
        while (!open.empty()) {
            IndexType next = open.front();
            open.pop();
            for (auto[neighbor, edge] : graph.edges_from(next)) {
                if (visited.contains(neighbor)) {
                    continue;
                }
                visited.emplace(neighbor);

                onDiscovered(
                    static_cast<IndexType>(next),
                    static_cast<IndexType>(neighbor),
                    static_cast<VertexType>(*graph.vertex(neighbor)),
                    static_cast<EdgeType>(edge)
                );
                open.push(neighbor);
            }
        }
    }
}
#endif //GRAPPHS_FLOOD_H
