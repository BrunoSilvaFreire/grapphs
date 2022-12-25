#include <iostream>
#include <fstream>
#include <grapphs/osm/parse.h>
#include <grapphs/svg.h>


class AABB {
private:
    double minX, minY, maxX, maxY;
public:
    AABB(
        double minX = std::numeric_limits<double>::max(),
        double minY = std::numeric_limits<double>::max(),
        double maxX = -std::numeric_limits<double>::max(),
        double maxY = -std::numeric_limits<double>::max())
        : minX(minX), minY(minY), maxX(maxX), maxY(maxY) {
    }

    double get_min_x() const {
        return minX;
    }

    void set_min_x(double minX) {
        AABB::minX = minX;
    }

    double get_min_y() const {
        return minY;
    }

    void set_min_y(double minY) {
        AABB::minY = minY;
    }

    double get_max_x() const {
        return maxX;
    }

    void set_max_x(double maxX) {
        AABB::maxX = maxX;
    }

    double get_max_y() const {
        return maxY;
    }

    void set_max_y(double maxY) {
        AABB::maxY = maxY;
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
    gpp::osm::OSMGraph graph;

    gpp::osm::parse(file, graph);

    std::cout << "Graph size: " << graph.size() << std::endl;

    AABB aabb;
    for (std::size_t i = 0; i < graph.size(); ++i) {
        gpp::osm::Node* pNode = graph.vertex(i);
        const gpp::osm::Coordinate& coordinate = pNode->get_location();
        double x = coordinate.get_longitude();
        double y = coordinate.get_latitude();
        if (x > aabb.get_max_x()) {
            aabb.set_max_x(x);
        }
        if (y > aabb.get_max_y()) {
            aabb.set_max_y(y);
        }
        if (x < aabb.get_min_x()) {
            aabb.set_min_x(x);
        }
        if (y < aabb.get_min_y()) {
            aabb.set_min_y(y);
        }
    }
    std::cout << "AABB min X: " << aabb.get_min_x() << std::endl;
    std::cout << "AABB max X: " << aabb.get_max_x() << std::endl;
    std::cout << "AABB min Y: " << aabb.get_min_y() << std::endl;
    std::cout << "AABB max Y: " << aabb.get_max_y() << std::endl;
    gpp::SVGViewBox viewBox = gpp::SVGViewBox::centralized(4000, 4000);
    AABB viewportAABB(
        viewBox.minX, viewBox.minY, viewBox.minX + viewBox.width, viewBox.minY + viewBox.height
    );
    gpp::SVGWriter<gpp::osm::OSMGraph> writer(
        viewBox, [&](
            std::size_t index, const gpp::osm::Node& node, float& x, float& y
        ) {
            const gpp::osm::Coordinate& location = node.get_location();
            double relX = inv_lerp(aabb.get_min_x(), aabb.get_max_x(), location.get_longitude());
            double relY = inv_lerp(aabb.get_min_y(), aabb.get_max_y(), location.get_latitude());
            x = lerp(viewportAABB.get_min_x(), viewportAABB.get_max_x(), relX);
            y = lerp(viewportAABB.get_min_y(), viewportAABB.get_max_x(), relY);
        }
    );
    writer.set_flags(gpp::SVGWriterFlags::eDrawEdges);

    auto svgFile = src / "curitiba.svg";
    {
        std::ofstream stream(svgFile);
        writer.write(stream, graph);
        std::cout << "SVG File: " << std::filesystem::absolute(svgFile) << std::endl;
    }
}