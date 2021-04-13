#ifndef GRAPPHS_GRAPH_H
#define GRAPPHS_GRAPH_H

#include <unordered_map>
#include <functional>
#include <iterator>

namespace gpp {
    typedef size_t DefaultGraphIndex;

    template<
            typename V,
            typename E,
            typename Derived,
            typename GraphIndex = DefaultGraphIndex
    >
    class Graph {
    public:
        virtual V *vertex(GraphIndex index) = 0;

        virtual E *edge(GraphIndex from, GraphIndex to) = 0;

        virtual GraphIndex size() = 0;

        virtual void connect(GraphIndex from, GraphIndex to, E edge) = 0;

        bool try_get_vertex(GraphIndex index, V &output) {
            V *ptr = vertex(index);
            if (ptr != nullptr) {
                output = *ptr;
            }
            return ptr != nullptr;
        }

        bool try_get_edge(GraphIndex index, GraphIndex to, E &output) {
            E *ptr = edge(index, to);
            if (ptr != nullptr) {
                output = *ptr;
            }
            return ptr != nullptr;
        }


        typename Derived::EdgeView edges_from(GraphIndex vertex) = 0;
    };


}
#endif
