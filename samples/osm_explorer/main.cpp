#include <iostream>
#include <grapphs/osm/parse.h>
#include <grapphs/dot.h>


class AABB {
private:
    double minX, minY, maxX, maxY;
public:
    AABB(
        double minX = std::numeric_limits<double>::max(),
        double minY = std::numeric_limits<double>::max(),
        double maxX = std::numeric_limits<double>::min(),
        double maxY = std::numeric_limits<double>::min())
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
    int maxExportIndex = 50;
    if (argc > 1) {
        maxExportIndex = atoi(argv[1]);
    }

    gpp::osm::OSMGraph graph;

    gpp::osm::parse(file, graph);

    std::cout << "Graph size: " << graph.size();

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

    gpp::GraphWriter<gpp::osm::OSMGraph> writer;

    writer.setVertexPredicate(
        [&](std::size_t index, const gpp::osm::Node& vertex) {
            return index < maxExportIndex;
        }
    );
    writer.setEdgePredicate(
        [](std::size_t from, std::size_t to, const gpp::osm::Way& edge) {
            return false;
        }
    );
    float mapSize = 500;
    writer.setVertexWriter(
        [&](
            std::stringstream& stream, std::size_t index, const gpp::osm::Node& vertex
        ) {
            const gpp::osm::Coordinate& coordinate = vertex.get_location();
            double x = coordinate.get_longitude();
            double y = coordinate.get_latitude();

            double fx = lerp<double>(0.0, mapSize, inv_lerp(aabb.get_min_x(), aabb.get_max_x(), x));
            double fy = lerp<double>(0.0, mapSize, inv_lerp(aabb.get_min_y(), aabb.get_max_y(), y));

            stream << " [shape=point width=0.005 label=\"\" pos=\"" << fx << "," << fy << "!\""
                   << "];";
        }
    );

    writer.save_to_dot(graph, src / "curitiba.dot");
}