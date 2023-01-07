//
// Created by Bruno Freire on 24/12/22.
//

#ifndef GRAPPHS_SVG_H
#define GRAPPHS_SVG_H

#include <ostream>
#include <functional>
#include <grapphs/adjacency_list.h>

namespace gpp {
    struct svg_viewbox {
    public:
        float minX, minY, width, height;

        svg_viewbox(float minX, float minY, float width, float height);

        static svg_viewbox centralized(float width, float height);

    };

    enum svg_writer_flags : std::uint8_t {
        VERBOSE = 1 << 0,
        DRAW_VERTICES = 1 << 1,
        DRAW_EDGES = 1 << 2
    };

    union svg_color {
        struct {
            uint8_t r, g, b, a;
        };
        uint8_t rgba[4];
        uint32_t rgba32;

        svg_color(uint32_t rgba) : rgba32(rgba) {
        }

        static svg_color black();

        friend std::ostream& operator<<(std::ostream& os, const svg_color& color);

    };

    struct SVGAttributes {
        svg_color color = svg_color::black();
        float size = 1;

        friend std::ostream& operator<<(std::ostream& os, const SVGAttributes& attributes);
    };

    template<typename t_graph>
    class svg_writer {
    public:
        using index_type = typename t_graph::index_type;
        using vertex_type = typename t_graph::vertex_type;
        using edge_type = typename t_graph::edge_type;

        using position_functor = std::function<
            void(
                index_type i, const vertex_type& vertex, float& x, float& y
            )
        >;

        using vertex_filter = std::function<
            bool(
                index_type i, const vertex_type& vertex
            )
        >;

        using edge_filter = std::function<
            bool(
                index_type from, index_type to, const edge_type& edgeType
            )
        >;

        template<typename TElement> using customizer = std::function<
            void(
                const TElement& entry, SVGAttributes& attributes
            )
        >;

        using vertex_customizer = customizer<vertex_type>;
        using edge_customizer = customizer<edge_type>;
    private:
        svg_viewbox _viewBox;
        float _nodeRadius = 1;
        svg_writer_flags _flags = static_cast<svg_writer_flags>(
            svg_writer_flags::DRAW_EDGES | svg_writer_flags::DRAW_VERTICES
        );
        position_functor _positionFunctor;
        vertex_filter _vertexFilter;
        edge_filter _edgeFilter;
        vertex_customizer _vertexCustomizer;
        edge_customizer _edgeCustomizer;

        bool test_flags(svg_writer_flags flags) {
            return (_flags & flags) == flags;
        }

    public:
        svg_writer(
            svg_viewbox viewBox, const position_functor& positionFunctor
        ) : _viewBox(viewBox), _positionFunctor(positionFunctor) {
        }

        void set_node_radius(float nodeRadius) {
            _nodeRadius = nodeRadius;
        }

        void set_flags(svg_writer_flags flags) {
            _flags = flags;
        }

        void set_vertex_filter(const vertex_filter& vertexFilter) {
            _vertexFilter = vertexFilter;
        }

        void set_edge_filter(const edge_filter& edgeFilter) {
            _edgeFilter = edgeFilter;
        }

        void set_vertex_customizer(const vertex_customizer& vertexCustomizer) {
            _vertexCustomizer = vertexCustomizer;
        }

        void set_edge_customizer(const edge_customizer& edgeCustomizer) {
            _edgeCustomizer = edgeCustomizer;
        }

        void write(
            std::ostream& os, const gpp::adjacency_list<vertex_type, edge_type, index_type>& graph
        ) {
            bool verbose = test_flags(svg_writer_flags::VERBOSE);
            os << "<svg viewBox=\"" << _viewBox.minX << " " << _viewBox.minY << " "
               << _viewBox.width << " " << _viewBox.height
               << R"(" xmlns="http://www.w3.org/2000/svg">)" << std::endl;

            if (test_flags(svg_writer_flags::DRAW_VERTICES)) {
                for (index_type i = 0; i < graph.size(); i++) {
                    const vertex_type& vertex = *graph.vertex(i);
                    if (_vertexFilter != nullptr && !_vertexFilter(i, vertex)) {
                        continue;
                    }

                    float x, y;
                    _positionFunctor(i, vertex, x, y);

                    if (verbose) {
                        os << "<!-- " << i << " --> " << std::endl;
                    }

                    SVGAttributes attributes;
                    attributes.size = _nodeRadius;

                    if (_vertexCustomizer != nullptr) {
                        _vertexCustomizer(vertex, attributes);
                    }

                    os << "<circle cx=\"" << x << "\" cy=\"" << y << "\" r=\"" << attributes.size
                       << "\" fill=\"" << attributes.color << "\"/> " << std::endl;
                }
            }
            if (test_flags(svg_writer_flags::DRAW_EDGES)) {
                for (index_type i = 0; i < graph.size(); ++i) {
                    const vertex_type& fromVertex = *graph.vertex(i);

                    float x1, y1;
                    _positionFunctor(i, fromVertex, x1, y1);
                    for (const auto& [j, edge] : graph.edges_from(i)) {
                        float x2, y2;
                        const vertex_type& toVertex = *graph.vertex(j);

                        if (_edgeFilter != nullptr && !_edgeFilter(i, j, edge)) {
                            continue;
                        }

                        _positionFunctor(j, toVertex, x2, y2);
                        if (verbose) {
                            os << "<!-- " << i << "-" << j << " --> " << std::endl;
                        }
                        SVGAttributes attributes;
                        attributes.size = _nodeRadius;

                        if (_edgeCustomizer != nullptr) {
                            _edgeCustomizer(edge, attributes);
                        }
                        os << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2
                           << "\" y2=\"" << y2 << "\" stroke=\"" << attributes.color
                           << "\" stroke-width=\"" << attributes.size << "\"/>" << std::endl;
                    }
                }
            }
            os << "</svg>" << std::endl;
        }
    };

    template<typename t_graph>
    void to_svg(
        std::size_t viewboxHeight,
        std::size_t viewboxWidth,
        const t_graph& graph,
        std::ostream& stream
    ) {
        using index_type = typename t_graph::index_type;
        using vertex_type = typename t_graph::vertex_type;
        using edge_type = typename t_graph::edge_type;

        gpp::graph<typename t_graph::vertex_type, typename t_graph::edge_type> a;

        for (index_type i = 0; i < a.size(); ++i) {
            const vertex_type& vertex = a.vertex(i);
        }
    }
}
#endif //GRAPPHS_SVG_H
