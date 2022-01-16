#include <grapphs/tests/mazes.h>

namespace gpp {

    Maze::Maze(const std::filesystem::path& jsonPath) {
        if (!std::filesystem::exists(jsonPath)) {
            std::stringstream stream;
            stream << "json path '" << std::filesystem::absolute(jsonPath) << "'does not exist";
            throw std::runtime_error(stream.str());
        }
        std::fstream file(jsonPath);
        nlohmann::json json;
        file >> json;
        auto vertices = json["vertices"].get<std::vector<nlohmann::json >>();
        for (size_t i = 0; i < vertices.size(); ++i) {
            const auto& vert = vertices[i];
            int x = vert["x"].get<int>();
            int y = vert["y"].get<int>();
            graph.push(Cell(x, y));
        }
        auto edges = json["edges"].get<nlohmann::json>();
        for (const auto &[key, edge] : edges.items()) {
            std::size_t index = std::stoul(key);
            for (std::size_t to : edge.get<std::vector<std::size_t >>()) {
                graph.connect(index, to, 1);
            }
        }
        start = json["start"].get<std::size_t>();
        end = json["end"].get<std::size_t>();
        size = json["size"].get<std::size_t>();
        shortestPath = json["shortest_path"].get<std::vector<size_t >>();
    }

    const gpp::AdjacencyList<Cell, int>& Maze::getGraph() const {
        return graph;
    }

    size_t Maze::getStart() const {
        return start;
    }

    size_t Maze::getEnd() const {
        return end;
    }

    size_t Maze::getSize() const {
        return size;
    }

    const std::vector<size_t>& Maze::getShortestPath() const {
        return shortestPath;
    }

    std::ostream& operator<<(std::ostream& os, const Cell& cell) {
        os << "x: " << cell.x << " y: " << cell.y;
        return os;
    }

    Cell::Cell(int x, int y) : x(x), y(y) {}

    void test_mazes(const std::function<void(Maze&)>& block) {
        auto wdir = std::filesystem::current_path();
        std::filesystem::path mazesDir = wdir / "mazes";
        std::filesystem::directory_iterator ot(mazesDir);
        for (auto entry : ot) {
            GTEST_LOG_(INFO) << "Reading maze " << entry.path() << " (" << entry.file_size()
                             << " bytes).";

            std::filesystem::path jsonPath = entry.path();
            Maze m(jsonPath);
            block(m);

        }
    }
}