#ifndef GRAPPHS_DOT_H
#define GRAPPHS_DOT_H

#include <string>
#include <grapphs/graph.h>
#include <grapphs/adjacency_list.h>
#include <sstream>
#include <fstream>
#include <filesystem>

namespace gpp {
    template<typename VertexType, typename EdgeType, typename IndexType = DefaultGraphIndex>
    std::string to_dot(const gpp::AdjacencyList<VertexType, EdgeType, IndexType>& graph) {
        std::stringstream stream;
        stream << "digraph {" << std::endl;
        for (const auto &[vertexPtr, index]: graph.all_vertices()) {
            stream << std::string(4, ' ') << index << " [shape=box label=\"#" << index << ": "
                   << (*vertexPtr) << '"' <<
                   "];" << std::endl;
        }
        for (auto[vertex, index]: graph.all_vertices()) {
            for (auto[otherIndex, edge]: graph.edges_from(index)) {
                stream << std::string(4, ' ') << index << " -> " << otherIndex /*<< " [label=\"" << index << "->"
                       << otherIndex << ": " << edge << '"' << "];"*/ << std::endl;
            }
        }
        stream << "}";
        return stream.str();
    }

    template<typename VertexType, typename EdgeType, typename IndexType = DefaultGraphIndex>
    bool save_to_dot(
        const gpp::AdjacencyList<VertexType, EdgeType, IndexType>& graph,
        const std::filesystem::path& outputPath
    ) {
        std::ofstream file(outputPath);
        if (!file.is_open()) {
            return false;
        }
        file << gpp::to_dot(graph);
        file.close();
        return true;
    }
}
#endif
