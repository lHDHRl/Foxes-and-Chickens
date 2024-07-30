#include "Pathfinding.h"
#include <queue>
#include <algorithm>
#include <iostream>

std::vector<sf::Vector2i> bfsPathfinding(const sf::Vector2i& start, const sf::Vector2i& goal, int rows, int cols, const std::vector<std::vector<bool>>& gridOccupancy) {
    if (start.x < 0 || start.y < 0 || start.x >= cols || start.y >= rows ||
        goal.x < 0 || goal.y < 0 || goal.x >= cols || goal.y >= rows) {
        std::cerr << "Error: Start or goal position out of bounds!" << std::endl;
        return {};
    }

    if (gridOccupancy[start.y][start.x] || gridOccupancy[goal.y][goal.x]) {
        std::cerr << "Error: Start or goal position is blocked!" << std::endl;
        return {};
    }

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::vector<std::vector<sf::Vector2i>> parent(rows, std::vector<sf::Vector2i>(cols, { -1, -1 }));
    std::queue<sf::Vector2i> queue;
    std::vector<sf::Vector2i> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} }; // Right, Down, Left, Up

    queue.push(start);
    visited[start.y][start.x] = true;

    while (!queue.empty()) {
        sf::Vector2i current = queue.front();
        queue.pop();

        if (current == goal) {
            std::vector<sf::Vector2i> path;
            for (sf::Vector2i step = goal; step != start; step = parent[step.y][step.x]) {
                if (step == sf::Vector2i(-1, -1)) {
                    std::cerr << "Error: No path found!" << std::endl;
                    return {};
                }
                path.push_back(step);
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (const auto& dir : directions) {
            sf::Vector2i neighbor = current + dir;

            if (neighbor.x >= 0 && neighbor.x < cols &&
                neighbor.y >= 0 && neighbor.y < rows &&
                !visited[neighbor.y][neighbor.x] &&
                !gridOccupancy[neighbor.y][neighbor.x]) {

                visited[neighbor.y][neighbor.x] = true;
                parent[neighbor.y][neighbor.x] = current;
                queue.push(neighbor);
            }
        }
    }

    std::cerr << "Error: No path found!" << std::endl;
    return {};
}
