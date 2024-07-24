#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Cell.h"

class Grid {
public:
    Grid(int rows, int cols, int cellSize, const sf::Texture& texture);
    void draw(sf::RenderWindow& window);

private:
    int rows;
    int cols;
    int cellSize;
    std::vector<Cell> cells;
    void createGrid(const sf::Texture& texture);
};