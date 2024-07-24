#include "Cell.h"

Cell::Cell(float x, float y, float size, const sf::Texture& texture) {
    shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(size, size));
    shape.setTexture(&texture);
}

void Cell::draw(sf::RenderWindow& window) {
    window.draw(shape);
}