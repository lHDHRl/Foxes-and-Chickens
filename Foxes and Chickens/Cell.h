#pragma once

#include <SFML/Graphics.hpp>

class Cell {
public:
    Cell(float x, float y, float size);
    void setTexture(const sf::Texture* texture);
    void draw(sf::RenderWindow& window);

private:
    sf::RectangleShape border;
    sf::Sprite sprite;
    float size;
};
