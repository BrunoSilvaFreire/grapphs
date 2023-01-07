#ifndef GRAPPHS_DOT_H
#define GRAPPHS_DOT_H

#include <string>
#include <grapphs/graph.h>
#include <grapphs/adjacency_list.h>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <grapphs/graph_writer.h>

namespace gpp {
    template<typename t_graph>
    std::string to_dot(const t_graph& graph) {
        gpp::graph_writer<decltype(graph)> writer;
        return writer.to_dot(graph);
    }

    template<typename t_graph>
    bool save_to_dot(
        const t_graph& graph,
        const std::filesystem::path& outputPath
    ) {
        gpp::graph_writer<decltype(graph)> writer;
        return writer.save_to_dot(graph, outputPath);
    }
}
#endif
