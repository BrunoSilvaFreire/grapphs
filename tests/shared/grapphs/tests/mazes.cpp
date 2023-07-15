#include <grapphs/tests/mazes.h>

namespace gpp {

    maze::maze(const std::filesystem::path& jsonPath) {
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
            _graph.push(cell(x, y));
        }

        auto edges = json["edges"].get<nlohmann::json>();
        for (const auto& [key, edge] : edges.items()) {
            std::size_t index = std::stoul(key);
            for (std::size_t to : edge.get<std::vector<std::size_t >>()) {
                _graph.connect(index, to, 1);
            }
        }

        _start = json["start"].get<std::size_t>();
        _end = json["end"].get<std::size_t>();
        _size = json["size"].get<std::size_t>();
        _shortestPath = json["shortest_path"].get<std::vector<size_t >>();
    }

    const gpp::adjacency_list<cell, int>& maze::get_graph() const {
        return _graph;
    }

    size_t maze::get_start() const {
        return _start;
    }

    size_t maze::get_end() const {
        return _end;
    }

    size_t maze::get_size() const {
        return _size;
    }

    const std::vector<size_t>& maze::get_shortest_path() const {
        return _shortestPath;
    }

    std::ostream& operator<<(std::ostream& os, const cell& cell) {
        os << "x: " << cell.x << " y: " << cell.y;
        return os;
    }

    cell::cell(int x, int y) : x(x), y(y) {}

    void test_mazes(const std::function<void(maze&)>& block) {
        auto wdir = std::filesystem::current_path();
        std::filesystem::path mazesDir = wdir / "mazes";
        std::filesystem::directory_iterator ot(mazesDir);
        for (auto entry : ot) {
            GTEST_LOG_(INFO) << "Reading maze " << entry.path() << " (" << entry.file_size()
                             << " bytes).";

            std::filesystem::path jsonPath = entry.path();
            maze m(jsonPath);
            block(m);

        }
    }
}