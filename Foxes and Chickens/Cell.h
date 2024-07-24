#pragma once

#include <SFML/Graphics.hpp>

class Cell {
public:
    Cell(float x, float y, float size, const sf::Texture& texture);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape shape;
};