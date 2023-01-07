//
// Created by bruno on 16/01/2022.
//

#ifndef GRAPPHS_MAZES_H
#define GRAPPHS_MAZES_H

#include <grapphs/adjacency_list.h>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <gtest/gtest.h>

namespace gpp {
    struct Cell {
        int x, y;

        Cell(int x, int y);

        friend std::ostream& operator<<(std::ostream& os, const Cell& cell);
    };

    class maze {
    private:
        gpp::adjacency_list<Cell, int> _graph;
        std::size_t _start, _end;
        std::size_t _size;
        std::vector<size_t> _shortestPath;
    public:
        maze(const std::filesystem::path& jsonPath);

        const gpp::adjacency_list<Cell, int>& get_graph() const;

        size_t get_start() const;

        size_t get_end() const;

        size_t get_size() const;

        const std::vector<size_t>& get_shortest_path() const;
    };

    void test_mazes(const std::function<void(maze& maze)>& block);
}
#endif //GRAPPHS_MAZES_H
