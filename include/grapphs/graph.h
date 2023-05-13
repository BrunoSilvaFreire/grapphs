#ifndef GRAPPHS_GRAPH_H
#define GRAPPHS_GRAPH_H

#include <unordered_map>
#include <functional>
#include <iterator>

#if __cpp_concepts

#include <concepts>

#endif

namespace gpp {
    typedef size_t default_graph_index;

    template<typename t_vertex, typename t_edge, typename t_index = default_graph_index>
    class graph {
    public:

        using vertex_type = t_vertex;
        using edge_type = t_edge;
        using index_type = t_index;

        /**
         * @returns The number of vertices in this graph.
         */
        virtual index_type size() const = 0;

        /**
         * @returns A pointer to the vertex corresponding to the given index, or nullptr is none is found.
         */
        virtual vertex_type* vertex(index_type index) = 0;

        /**
         * @returns A const pointer to the vertex corresponding to the given index,
         * or nullptr is none is found.
         */
        virtual const vertex_type* vertex(index_type index) const = 0;

        /**
         * @returns A pointer to the edge connecting the vertex corresponding to the 'from' index,
         * to the vertex corresponding to the 'to' index, or nullptr is none is found.
         */
        virtual edge_type* edge(index_type from, index_type to) = 0;

        /**
         * Connects the given vertices using a one way connection represented by the given
         * \p edge
         */
        virtual void connect(index_type from, index_type to, edge_type edge) = 0;

        /**
         * Disconnects the given vertices removing the previous edge that was present.
         */
        virtual bool disconnect(index_type from, index_type to) = 0;

        /**
         * Returns true if there are no vertices in this graph
         */
        bool empty() const {
            return size() == 0;
        }

        bool try_get_vertex(index_type index, vertex_type& output) {
            vertex_type* ptr = vertex(index);
            if (ptr != nullptr) {
                output = *ptr;
            }
            return ptr != nullptr;
        }

        bool try_get_edge(index_type index, index_type to, edge_type& output) {
            edge_type* ptr = edge(index, to);
            if (ptr != nullptr) {
                output = *ptr;
            }
            return ptr != nullptr;
        }
    };

#if __cpp_concepts

    template<typename graph_type>
    concept is_graph = std::is_base_of_v<
        gpp::graph<
            typename graph_type::vertex_type,
            typename graph_type::edge_type,
            typename graph_type::index_type
        >,
        graph_type
    >;

#endif

}
#endif
