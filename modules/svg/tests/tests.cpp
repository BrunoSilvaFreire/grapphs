#include <gtest/gtest.h>
#include <grapphs/svg.h>
#include <grapphs/tests/mazes.h>

TEST(grapphs_svg, to_svg) {
    std::filesystem::path jsonPath = std::filesystem::current_path() / "maze.json";

    std::array<gpp::svg_color, 4> colors = {
        gpp::svg_color(0xFFFF0000),
        gpp::svg_color(0xFF00FF00),
        gpp::svg_color(0xFF00FFFF),
        gpp::svg_color(0xFF0000FF)
    };

    gpp::test_mazes([&](gpp::maze& maze) {
        auto graph = maze.get_graph();
        auto start = maze.get_start();
        auto end = maze.get_end();
        std::stringstream name;
        name << "maze_" << maze.get_size() << ".svg";
        auto path = std::set<std::size_t>(
            maze.get_shortest_path().begin(),
            maze.get_shortest_path().end()
        );
        gpp::svg_writer<decltype(graph)> writer(
            gpp::svg_viewbox(0, 0, maze.get_size(), maze.get_size()),
            [](
                const decltype(graph)::index_type i,
                const gpp::cell& vertex,
                float& x,
                float& y
            ) {
                x = vertex.x;
                y = vertex.y;
            }
        );
        writer.set_vertex_customizer(
            [&](
                std::size_t index,
                const gpp::cell& cell,
                gpp::svg_attributes& attributes
            ) {
                attributes.size = 0.33F;
                std::size_t numEdges = graph.edges_from(index).size();
                attributes.color = colors[numEdges - 1];
            }
        );
        writer.set_edge_customizer(
            [&](
                std::size_t from,
                std::size_t to,
                const float& cell,
                gpp::svg_attributes& attributes
            ) {
                if (path.find(from) == path.end() || path.find(to) == path.end()){
                    attributes.size = 0.25F;
                    attributes.color = gpp::svg_color(0XFF000000);
                } else {
                    attributes.size = 0.5F;
                    attributes.color = gpp::svg_color(0XFFFF00FF);
                }
            }
        );
        std::ofstream stream(std::filesystem::current_path() / name.str());
        writer.write(stream, graph);
    });
}
