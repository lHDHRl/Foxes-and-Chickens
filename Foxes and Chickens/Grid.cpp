#include "Grid.h"

Grid::Grid(int rows, int cols, int cellSize)
    : rows(rows), cols(cols), cellSize(cellSize) {
    createGrid();
}

void Grid::createGrid() {
    cells.resize(rows, std::vector<Cell>(cols, Cell(0, 0, cellSize)));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cells[i][j] = Cell(j * cellSize, i * cellSize, cellSize);
        }
    }
}

void Grid::setTexture(int row, int col, const sf::Texture* texture) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        cells[row][col].setTexture(texture);
    }
}

void Grid::draw(sf::RenderWindow& window) {
    for (auto& row : cells) {
        for (auto& cell : row) {
            cell.draw(window);
        }
    }
}
