#include "Grid.h"

Grid::Grid(int rows, int cols, int cellSize, const sf::Texture& texture)
    : rows(rows), cols(cols), cellSize(cellSize) {
    createGrid(texture);
}

void Grid::createGrid(const sf::Texture& texture) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cells.emplace_back(j * cellSize, i * cellSize, cellSize, texture);
        }
    }
}

void Grid::draw(sf::RenderWindow& window) {
    for (auto& cell : cells) {
        cell.draw(window);
    }
}