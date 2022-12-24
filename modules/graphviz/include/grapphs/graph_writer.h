#ifndef UNNECESSARYENGINE_GRAPH_WRITER_H
#define UNNECESSARYENGINE_GRAPH_WRITER_H

#include <string>
#include <sstream>
#include <functional>
#include <grapphs/adjacency_list.h>
#include <grapphs/adjacency_matrix.h>

namespace gpp {

    template<typename TGraph>
    class GraphWriter {
    public:
        using GraphType = std::remove_reference_t<TGraph>;
        using VertexWriter = std::function<void(
            std::stringstream& stream,
            typename GraphType::IndexType index,
            const typename GraphType::VertexType& vertex
        )>;
        using VertexPredicate = std::function<bool(
            typename GraphType::IndexType index,
            const typename GraphType::VertexType& vertex
        )>;
        using EdgeWriter = std::function<void(
            std::stringstream& stream,
            typename GraphType::IndexType from,
            typename GraphType::IndexType to,
            const typename GraphType::EdgeType& edge
        )>;
        using EdgePredicate = std::function<bool(
            typename GraphType::IndexType from,
            typename GraphType::IndexType to,
            const typename GraphType::EdgeType& edge
        )>;
    private:
        VertexWriter vertexWriter;
        EdgeWriter edgeWriter;
        VertexPredicate vertexPredicate;
        EdgePredicate edgePredicate;
        std::vector<std::string> notes;
    public:
        GraphWriter() {
            vertexWriter = [](
                std::stringstream& stream,
                typename GraphType::IndexType index,
                const typename GraphType::VertexType& vertex
            ) {
                stream << " [shape=box label=\"#" << index << ": " << vertex << '"' << "];";
            };
            edgeWriter = [](
                std::stringstream& stream,
                typename GraphType::IndexType from,
                typename GraphType::IndexType to,
                const typename GraphType::EdgeType& edge
            ) {
                // Hide by default
            };
        }

        void setVertexWriter(const VertexWriter& vertexWriter) {
            GraphWriter::vertexWriter = vertexWriter;
        }

        void setEdgeWriter(const EdgeWriter& edgeWriter) {
            GraphWriter::edgeWriter = edgeWriter;
        }

        void setVertexPredicate(const VertexPredicate& vertexPredicate) {
            GraphWriter::vertexPredicate = vertexPredicate;
        }

        void setEdgePredicate(const EdgePredicate& edgePredicate) {
            GraphWriter::edgePredicate = edgePredicate;
        }

        void addNote(const std::string& note) {
            notes.emplace_back(note);
        }

        std::string to_dot(const GraphType& graph) {
            std::stringstream stream;
            stream << "digraph {" << std::endl;
            for (const auto [vertexPtr, index] : graph.all_vertices()) {
                if (vertexPredicate != nullptr && !vertexPredicate(index, *vertexPtr)) {
                    continue;
                }
                stream << std::string(4, ' ') << index;
                vertexWriter(stream, index, *vertexPtr);
                stream << std::endl;
            }
            for (auto [vertex, fromIndex] : graph.all_vertices()) {
                for (auto [toIndex, edge] : graph.edges_from(fromIndex)) {
                    if (edgePredicate != nullptr && !edgePredicate(fromIndex, toIndex, edge)) {
                        continue;
                    }
                    stream << std::string(4, ' ') << fromIndex << " -> " << toIndex;
                    edgeWriter(stream, fromIndex, toIndex, edge);
                    stream << std::endl;
                }
            }
            if (!notes.empty()) {
                stream << "subgraph cluster_notes {" << std::endl;
                stream << "label = \"Notes\";" << std::endl;
                stream << "shape = rectangle;" << std::endl;
                stream << "color = black;" << std::endl;
                for (const auto& note : notes) {
                    stream << note << std::endl;
                }
                stream << "}" << std::endl;
            }
            stream << "}";
            return stream.str();
        }

        bool save_to_dot(
            const GraphType& graph,
            const std::filesystem::path& outputPath
        ) {
            std::ofstream file(outputPath);
            if (!file.is_open()) {
                return false;
            }
            file << to_dot(graph);
            file.close();
            return true;
        }
    };
}
#endif
