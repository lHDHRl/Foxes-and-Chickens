#include "Cell.h"

Cell::Cell(float x, float y, float size) : size(size) {
    sprite.setPosition(x, y);

    // ������������� �������
    border.setPosition(x, y);  // ��������� �������
    border.setSize(sf::Vector2f(size, size));  // ������ �������
    border.setFillColor(sf::Color::Transparent);  // ���������� ���
    border.setOutlineThickness(1);  // ������� �������
    border.setOutlineColor(sf::Color::Black);  // ���� �������
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
    window.draw(sprite);   // ������� ������ ������
    window.draw(border);  // ����� ������ �������
}
