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
    template<typename TGraph>
    std::string to_dot(const TGraph& graph) {
        gpp::GraphWriter<decltype(graph)> writer;
        return writer.to_dot(graph);
    }

    template<typename TGraph>
    bool save_to_dot(
        const TGraph& graph,
        const std::filesystem::path& outputPath
    ) {
        gpp::GraphWriter<decltype(graph)> writer;
        return writer.save_to_dot(graph, outputPath);
    }
}
#endif
