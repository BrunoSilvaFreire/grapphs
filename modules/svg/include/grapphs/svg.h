//
// Created by Bruno Freire on 24/12/22.
//

#ifndef GRAPPHS_SVG_H
#define GRAPPHS_SVG_H

#include <ostream>
#include <functional>
#include <grapphs/adjacency_list.h>

namespace gpp {
    struct SVGViewBox {
    public:
        float minX, minY, width, height;

        SVGViewBox(float minX, float minY, float width, float height);

        static SVGViewBox centralized(float width, float height);

    };

    enum SVGWriterFlags : std::uint8_t {
        eVerbose = 1 << 0, eDrawVertices = 1 << 1, eDrawEdges = 1 << 2
    };

    union SVGColor {
        struct {
            uint8_t r, g, b, a;
        };
        uint8_t rgba[4];
        uint32_t rgba32;

        SVGColor(uint32_t rgba) : rgba32(rgba) {
        }

        static SVGColor black();

        friend std::ostream& operator<<(std::ostream& os, const SVGColor& color);

    };

    struct SVGAttributes {
        SVGColor color = SVGColor::black();
        float size = 1;

        friend std::ostream& operator<<(std::ostream& os, const SVGAttributes& attributes);
    };

    template<typename TGraph>
    class SVGWriter {
    public:
        using IndexType = typename TGraph::IndexType;
        using VertexType = typename TGraph::VertexType;
        using EdgeType = typename TGraph::EdgeType;

        using PositionFunctor = std::function<
            void(
                IndexType i, const VertexType& vertex, float& x, float& y
            )
        >;

        using VertexFilter = std::function<
            bool(
                IndexType i, const VertexType& vertex
            )
        >;

        using EdgeFilter = std::function<
            bool(
                IndexType from, IndexType to, const EdgeType& edgeType
            )
        >;

        template<typename TElement> using Customizer = std::function<
            void(
                const TElement& entry, SVGAttributes& attributes
            )
        >;

        using VertexCustomizer = Customizer<VertexType>;
        using EdgeCustomizer = Customizer<EdgeType>;
    private:
        SVGViewBox _viewBox;
        float _nodeRadius = 1;
        PositionFunctor _positionFunctor;
        SVGWriterFlags _flags = static_cast<SVGWriterFlags>(SVGWriterFlags::eDrawEdges |
                                                            SVGWriterFlags::eDrawVertices);
        VertexFilter _vertexFilter;
        EdgeFilter _edgeFilter;
        VertexCustomizer _vertexCustomizer;
        EdgeCustomizer _edgeCustomizer;

        bool testFlags(SVGWriterFlags flags) {
            return (_flags & flags) == flags;
        }

    public:
        SVGWriter(
            SVGViewBox viewBox, const PositionFunctor& positionFunctor
        ) : _viewBox(viewBox), _positionFunctor(positionFunctor) {
        }

        void set_node_radius(float nodeRadius) {
            _nodeRadius = nodeRadius;
        }

        void set_flags(SVGWriterFlags flags) {
            _flags = flags;
        }

        void set_vertex_filter(const VertexFilter& vertexFilter) {
            _vertexFilter = vertexFilter;
        }

        void set_edge_filter(const EdgeFilter& edgeFilter) {
            _edgeFilter = edgeFilter;
        }

        void set_vertex_customizer(const VertexCustomizer& vertexCustomizer) {
            _vertexCustomizer = vertexCustomizer;
        }

        void set_edge_customizer(const EdgeCustomizer& edgeCustomizer) {
            _edgeCustomizer = edgeCustomizer;
        }

        void write(
            std::ostream& os, const gpp::AdjacencyList<VertexType, EdgeType, IndexType>& graph
        ) {
            bool verbose = testFlags(SVGWriterFlags::eVerbose);
            os << "<svg viewBox=\"" << _viewBox.minX << " " << _viewBox.minY << " "
               << _viewBox.width << " " << _viewBox.height
               << R"(" xmlns="http://www.w3.org/2000/svg">)" << std::endl;

            if (testFlags(SVGWriterFlags::eDrawVertices)) {
                for (IndexType i = 0; i < graph.size(); i++) {
                    const VertexType& vertex = *graph.vertex(i);
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
            if (testFlags(SVGWriterFlags::eDrawEdges)) {
                for (IndexType i = 0; i < graph.size(); ++i) {
                    const VertexType& fromVertex = *graph.vertex(i);

                    float x1, y1;
                    _positionFunctor(i, fromVertex, x1, y1);
                    for (const auto& [j, edge] : graph.edges_from(i)) {
                        float x2, y2;
                        const VertexType& toVertex = *graph.vertex(j);

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

    template<typename TGraph>
    void to_svg(
        std::size_t viewboxHeight,
        std::size_t viewboxWidth,
        const TGraph& graph,
        std::ostream& stream
    ) {
        using IndexType = typename TGraph::IndexType;
        using VertexType = typename TGraph::VertexType;
        using EdgeType = typename TGraph::EdgeType;

        gpp::Graph<typename TGraph::VertexType, typename TGraph::EdgeType> a;

        for (IndexType i = 0; i < a.size(); ++i) {
            const VertexType& vertex = a.vertex(i);
        }
    }
}
#endif //GRAPPHS_SVG_H
