#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Grid {
public:
    Grid(int rows, int cols, int cellSize);
    void draw(sf::RenderWindow& window);

private:
    int rows;
    int cols;
    int cellSize;
    std::vector<sf::Vertex> lines;
    void createGrid();
};
