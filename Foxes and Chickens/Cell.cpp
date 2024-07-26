#include "Cell.h"

Cell::Cell(float x, float y, float size) : size(size) {
    sprite.setPosition(x, y);

    // Инициализация границы
    border.setPosition(x, y);  // Начальная позиция
    border.setSize(sf::Vector2f(size, size));  // Размер границы
    border.setFillColor(sf::Color::Transparent);  // Прозрачный фон
    border.setOutlineThickness(1);  // Толщина границы
    border.setOutlineColor(sf::Color::Black);  // Цвет границы
}

void Cell::setTexture(const sf::Texture* texture) {
    sprite.setTexture(*texture);

    if (texture) {
        sf::Vector2u textureSize = texture->getSize();
        sprite.setTextureRect(sf::IntRect(0, 0, textureSize.x, textureSize.y));
        sprite.setScale(size / textureSize.x, size / textureSize.y);
    }
}

void Cell::draw(sf::RenderWindow& window) {
    window.draw(sprite);   // Сначала рисуем спрайт
    window.draw(border);  // Затем рисуем границу
}
