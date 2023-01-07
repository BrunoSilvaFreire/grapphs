#include <iostream>
#include <fstream>
#include <grapphs/osm/parse.h>
#include <grapphs/svg.h>

class aabb {
private:
    double _minX, _minY, _maxX, _maxY;
public:
    aabb(
        double minX = std::numeric_limits<double>::max(),
        double minY = std::numeric_limits<double>::max(),
        double maxX = -std::numeric_limits<double>::max(),
        double maxY = -std::numeric_limits<double>::max())
        : _minX(minX), _minY(minY), _maxX(maxX), _maxY(maxY) {
    }

    double get_min_x() const {
        return _minX;
    }

    void set_min_x(double minX) {
        aabb::_minX = minX;
    }

    double get_min_y() const {
        return _minY;
    }

    void set_min_y(double minY) {
        aabb::_minY = minY;
    }

    double get_max_x() const {
        return _maxX;
    }

    void set_max_x(double maxX) {
        aabb::_maxX = maxX;
    }

    double get_max_y() const {
        return _maxY;
    }

    void set_max_y(double maxY) {
        aabb::_maxY = maxY;
    }
};

template<typename T>
static constexpr T lerp(T x, T y, T t) {
    return static_cast<T>((x + (y - x) * t));
}

template<typename T>
static float inv_lerp(T x, T y, T value) {
    if (x < 0) {
        y -= x;
        value -= x;
        x = 0;
    }
    return (value - x) / (y - x);
}

int main(int argc, char** argv) {
    std::filesystem::path src = std::filesystem::current_path();
    std::filesystem::path file = src / "curitiba.xml.osm";
    if (!std::filesystem::exists(file)) {
        std::cerr << "Unable to find curitiba file @ " << std::filesystem::absolute(file)
                  << std::endl;
        return 1;
    }
    gpp::osm::osm_graph graph;

    gpp::osm::parse(file, graph);

    std::cout << "Graph size: " << graph.size() << std::endl;

    aabb cityAabb;
    for (std::size_t i = 0; i < graph.size(); ++i) {
        gpp::osm::osm_node* pNode = graph.vertex(i);
        const gpp::osm::Coordinate& coordinate = pNode->get_location();
        double x = coordinate.get_longitude();
        double y = coordinate.get_latitude();
        if (x > cityAabb.get_max_x()) {
            cityAabb.set_max_x(x);
        }
        if (y > cityAabb.get_max_y()) {
            cityAabb.set_max_y(y);
        }
        if (x < cityAabb.get_min_x()) {
            cityAabb.set_min_x(x);
        }
        if (y < cityAabb.get_min_y()) {
            cityAabb.set_min_y(y);
        }
    }
    std::cout << "AABB min X: " << cityAabb.get_min_x() << std::endl;
    std::cout << "AABB max X: " << cityAabb.get_max_x() << std::endl;
    std::cout << "AABB min Y: " << cityAabb.get_min_y() << std::endl;
    std::cout << "AABB max Y: " << cityAabb.get_max_y() << std::endl;

    gpp::svg_viewbox viewBox = gpp::svg_viewbox::centralized(4000, 4000);
    aabb viewportAabb(
        viewBox.minX, viewBox.minY, viewBox.minX + viewBox.width, viewBox.minY + viewBox.height
    );

    gpp::svg_writer<gpp::osm::osm_graph> writer(
        viewBox, [&](
            std::size_t index, const gpp::osm::osm_node& node, float& x, float& y
        ) {
            const gpp::osm::Coordinate& location = node.get_location();

            double relX = inv_lerp(cityAabb.get_min_x(), cityAabb.get_max_x(), location.get_longitude());
            double relY = inv_lerp(cityAabb.get_min_y(), cityAabb.get_max_y(), location.get_latitude());

            x = static_cast<float>(lerp(viewportAabb.get_min_x(), viewportAabb.get_max_x(), relX));
            y = static_cast<float>(lerp(viewportAabb.get_min_y(), viewportAabb.get_max_x(), relY));
        }
    );

    writer.set_flags(gpp::svg_writer_flags::DRAW_EDGES);

    writer.set_edge_filter(
        [&](std::size_t from, std::size_t to, const gpp::osm::way& way) {
            gpp::osm::way_metadata meta;
            if (!graph.get_metadata(way, meta)) {
                return true;
            }
            return (meta.get_flags() & gpp::osm::way_metadata::flags::BUILDING)
                   != gpp::osm::way_metadata::flags::BUILDING;
        }
    );

    writer.set_edge_customizer(
        [&](const gpp::osm::way& way, gpp::SVGAttributes& attributes) {

            gpp::osm::way_metadata meta;
            if (!graph.get_metadata(way, meta)) {
                return;
            }
            attributes.color.r = static_cast<uint8_t>(lerp<float>(
                0, 255, inv_lerp<float>(
                    0, 80, meta.get_max_speed())));

            switch (meta.get_kind()) {
                case gpp::osm::way_metadata::kind::WAY:
                    attributes.size = 0.5F;
                    break;
                case gpp::osm::way_metadata::kind::ROAD:
                    attributes.size = 1.0F;
                    break;
                case gpp::osm::way_metadata::kind::AVENUE:
                    attributes.size = 1.5F;
                    break;
                case gpp::osm::way_metadata::kind::HIGHWAY:
                    attributes.size = 2.0F;
                    break;
                default:
                    break;
            }
        }
    );
    auto svgFile = src / "curitiba.svg";
    {
        std::ofstream stream(svgFile);
        writer.write(stream, graph);
        std::cout << "SVG File: " << std::filesystem::absolute(svgFile) << std::endl;
    }
}