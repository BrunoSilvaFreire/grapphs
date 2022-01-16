#!/usr/bin/python3
import os.path

import sys

from mazelib import Maze
from mazelib.generate.Prims import Prims
from mazelib.solve.ShortestPath import ShortestPath
from numpy import ndarray
import json
import argparse

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('--size', metavar='N', type=int, help='an integer for the accumulator')
parser.add_argument('--monteMazes', nargs='?', type=int, default=10)
parser.add_argument('--monteAttempts', nargs='?', type=int, default=10)
parser.add_argument('--output', nargs='?', type=str, default=None)
parser.add_argument('--reuse', nargs='?', type=bool, default=False)
args = parser.parse_args()
if args.output is not None and args.reuse is not None and args.reuse:
    if os.path.exists(args.output):
        print(f"Maze {args.output} already exists and reuse is specified.", file=sys.stderr)
        exit(0)
halfSize = args.size
size = halfSize * 2 + 1
m = Maze()
m.generator = Prims(halfSize, halfSize)
m.solver = ShortestPath()
monte_nmazes = args.monteMazes
monte_nattempts = args.monteAttempts

m.generate_monte_carlo(monte_nmazes, monte_nattempts, 1.0)


def index_of(x: int, y: int):
    return y * size + x


globalToLocal = ndarray((size * size), dtype=int)
vertices = list()
edges = dict()

(sx, sy) = m.start
(ex, ey) = m.end
startIndex = index_of(sx, sy)
endIndex = index_of(ex, ey)


def out_of_bounds(nx, ny):
    return nx < 0 or ny < 0 or nx >= size or ny >= size


def is_start(x, y):
    return x == sx and y == sy


def is_end(x, y):
    return x == ex and y == ey


def is_walkable(x, y):
    if is_start(x, y) or is_end(x, y):
        return True
    return m.grid[x][y] == 0


for y in range(size):
    for x in range(size):
        index: int = index_of(x, y)
        if not is_walkable(x, y):
            continue
        localIndex = len(vertices)
        globalToLocal[index] = localIndex
        vertices.append(
            {
                "x": x,
                "y": y
            }
        )
for y in range(size):
    for x in range(size):
        index = index_of(x, y)
        if not is_walkable(x, y):
            continue
        diffs = [
            (-1, 0),
            (1, 0),
            (0, -1),
            (0, 1),
        ]
        for d in diffs:
            (dx, dy) = d
            nx = x + dx
            ny = y + dy
            nIndex = index_of(nx, ny)
            if out_of_bounds(nx, ny):
                continue
            nCell = m.grid[nx][ny]
            if not is_walkable(nx, ny):
                continue
            fromIndex = int(globalToLocal[index])
            toIndex = int(globalToLocal[nIndex])
            if fromIndex in edges:
                connectionList = edges[fromIndex]
            else:
                connectionList = list()
                edges[fromIndex] = connectionList
            connectionList.append(toIndex)

start = int(globalToLocal[startIndex])
end = int(globalToLocal[endIndex])
shortest_path = list()
shortest_path.append(start)
for (x, y) in m.solutions[0]:
    shortest_path.append(int(globalToLocal[index_of(x, y)]))
shortest_path.append(end)
graph = json.dumps({
    "size": size,
    "start": start,
    "end": end,
    "vertices": vertices,
    "edges": edges,
    "shortest_path": shortest_path
})
if args.output is None:
    print(graph)
else:
    output = open(args.output, "w")
    output.write(graph)
    output.close()
