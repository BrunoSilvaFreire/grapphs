#include <gtest/gtest.h>
#include <grapphs/tests/mazes.h>
#include <grapphs/tests/geometry.h>
#include <grapphs/algorithms/astar.h>

TEST(grapphs, astar) {
    gpp::test_mazes(
        [](gpp::maze& maze) {
            using index_type = gpp::default_graph_index;
            const gpp::adjacency_list<gpp::cell, int>& graph = maze.get_graph();
            gpp::graph_path path = gpp::astar(
                graph,
                maze.get_start(),
                maze.get_end(),
                [&](index_type from, index_type to) {
                    const gpp::cell* originCell = graph.vertex(from);
                    const gpp::cell* destinationCell = graph.vertex(to);
                    return distance(
                        static_cast<float>(originCell->x),
                        static_cast<float>(destinationCell->x),
                        static_cast<float>(originCell->y),
                        static_cast<float>(destinationCell->y)
                    );
                },
                [](index_type, index_type, int distance) {
                    return static_cast<float>(distance);
                }
            );
            const std::vector<size_t>& shortestPath = maze.get_shortest_path();
            EXPECT_EQ(path.count(), shortestPath.size())
                            << "AStar returned a different path from the shortest one";
            std::size_t max = std::min(path.count(), shortestPath.size());
            const std::vector<index_type>& vertices = path.get_vertices();
            for (std::size_t i = 0; i < max; ++i) {
                index_type actual = vertices[i];
                index_type expected = shortestPath[i];
                EXPECT_EQ(expected, actual)
                                << "Step #" << i << ", expected: " << expected << ", actual: "
                                << actual;
            }
        }
    );
}