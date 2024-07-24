#include "Grid.h"

Grid::Grid(int rows, int cols, int cellSize)
    : rows(rows), cols(cols), cellSize(cellSize) {
    createGrid();
}

void Grid::createGrid() {
    for (int i = 0; i <= rows; ++i) {
        lines.push_back(sf::Vertex(sf::Vector2f(0, i * cellSize), sf::Color::Black));
        lines.push_back(sf::Vertex(sf::Vector2f(cols * cellSize, i * cellSize), sf::Color::Black));
    }
    for (int i = 0; i <= cols; ++i) {
        lines.push_back(sf::Vertex(sf::Vector2f(i * cellSize, 0), sf::Color::Black));
        lines.push_back(sf::Vertex(sf::Vector2f(i * cellSize, rows * cellSize), sf::Color::Black));
    }
}

void Grid::draw(sf::RenderWindow& window) {
    window.draw(&lines[0], lines.size(), sf::Lines);
}