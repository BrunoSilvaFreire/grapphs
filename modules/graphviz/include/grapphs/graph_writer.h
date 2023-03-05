#ifndef GRAPPHS_GRAPH_WRITER_H
#define GRAPPHS_GRAPH_WRITER_H

#include <string>
#include <sstream>
#include <functional>

namespace gpp {

    template<typename t_graph>
    class graph_writer {
    public:
        using graph_type = std::remove_reference_t<t_graph>;
        using vertex_writer = std::function<
            void(
                std::stringstream& stream,
                typename graph_type::index_type index,
                const typename graph_type::vertex_type& vertex
            )
        >;
        using vertex_predicate = std::function<
            bool(
                typename graph_type::index_type index,
                const typename graph_type::vertex_type& vertex
            )
        >;
        using edge_writer = std::function<
            void(
                std::stringstream& stream,
                typename graph_type::index_type from,
                typename graph_type::index_type to,
                const typename graph_type::edge_type& edge
            )
        >;
        using edge_predicate = std::function<
            bool(
                typename graph_type::index_type from,
                typename graph_type::index_type to,
                const typename graph_type::edge_type& edge
            )
        >;
    private:
        vertex_writer _vertexWriter;
        edge_writer _edgeWriter;
        vertex_predicate _vertexPredicate;
        edge_predicate _edgePredicate;
        std::vector<std::string> _notes;
    public:
        graph_writer() {
            _vertexWriter = [](
                std::stringstream& stream,
                typename graph_type::index_type index,
                const typename graph_type::vertex_type& vertex
            ) {
                stream << " [shape=box label=\"#" << index << ": " << vertex << '"' << "];";
            };
            _edgeWriter = [](
                std::stringstream& stream,
                typename graph_type::index_type from,
                typename graph_type::index_type to,
                const typename graph_type::edge_type& edge
            ) {
                // Hide by default
            };
        }

        void set_vertex_writer(const vertex_writer& vertexWriter) {
            graph_writer::_vertexWriter = vertexWriter;
        }

        void set_edge_writer(const edge_writer& edgeWriter) {
            graph_writer::_edgeWriter = edgeWriter;
        }

        void set_vertex_predicate(const vertex_predicate& vertexPredicate) {
            graph_writer::_vertexPredicate = vertexPredicate;
        }

        void set_edge_predicate(const edge_predicate& edgePredicate) {
            graph_writer::_edgePredicate = edgePredicate;
        }

        void add_note(const std::string& note) {
            _notes.emplace_back(note);
        }

        std::string to_dot(const graph_type& graph) {
            std::stringstream stream;
            stream << "digraph {" << std::endl;
            for (const auto [index, vertexPtr] : graph.all_vertices()) {
                if (_vertexPredicate != nullptr && !_vertexPredicate(index, *vertexPtr)) {
                    continue;
                }
                stream << std::string(4, ' ') << index;
                _vertexWriter(stream, index, *vertexPtr);
                stream << std::endl;
            }
            for (auto [fromIndex, vertex] : graph.all_vertices()) {
                for (auto [toIndex, edge] : graph.edges_from(fromIndex)) {
                    if (_edgePredicate != nullptr && !_edgePredicate(fromIndex, toIndex, edge)) {
                        continue;
                    }
                    stream << std::string(4, ' ') << fromIndex << " -> " << toIndex;
                    _edgeWriter(stream, fromIndex, toIndex, edge);
                    stream << std::endl;
                }
            }
            if (!_notes.empty()) {
                stream << "subgraph cluster_notes {" << std::endl;
                stream << "label = \"Notes\";" << std::endl;
                stream << "shape = rectangle;" << std::endl;
                stream << "color = black;" << std::endl;
                for (const auto& note : _notes) {
                    stream << note << std::endl;
                }
                stream << "}" << std::endl;
            }
            stream << "}";
            return stream.str();
        }

        bool save_to_dot(
            const graph_type& graph,
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
