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

    class Maze {
    private:
        gpp::AdjacencyList<Cell, int> graph;
        std::size_t start, end;
        std::size_t size;
        std::vector<size_t> shortestPath;
    public:
        Maze(const std::filesystem::path& jsonPath);

        const gpp::AdjacencyList<Cell, int>& getGraph() const;

        size_t getStart() const;

        size_t getEnd() const;

        size_t getSize() const;

        const std::vector<size_t>& getShortestPath() const;
    };


    void test_mazes(const std::function<void(Maze& maze)>& block);
}
#endif //GRAPPHS_MAZES_H
