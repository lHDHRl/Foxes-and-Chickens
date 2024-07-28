#include "Fox.h"

Fox::Fox(float x, float y, sf::Texture* texture)
    : destination(x, y), speed(100.0f), moving(false) {
    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    sprite.setPosition(x, y);
    sprite.setOrigin(16, 16); // Центрирование спрайта
    sprite.setScale(2.0f, 2.0f); // Увеличение размера лисы в 2 раза
}

void Fox::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void Fox::startMoving(const sf::Vector2f& dest) {
    // Центрируем координаты в пределах ячейки
    destination.x = static_cast<int>(dest.x / 32) * 32 + 16;
    destination.y = static_cast<int>(dest.y / 32) * 32 + 16;
    moving = true;
    sprite.setTextureRect(sf::IntRect(32, 0, 32, 32)); // Запуск анимации движения
}

void Fox::update(float deltaTime) {
    if (moving) {
        sf::Vector2f direction = destination - sprite.getPosition();
        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 1.0f) {
            sf::Vector2f velocity = direction / distance * speed;
            sprite.move(velocity * deltaTime);
        } else {
            sprite.setPosition(destination);
            moving = false;
            sprite.setTextureRect(sf::IntRect(0, 0, 32, 32)); // Остановить анимацию на первом кадре
        }

        updateAnimation(deltaTime);
    }
}



bool Fox::isMoving() const {
    return moving;
}

sf::FloatRect Fox::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Fox::getPosition() const {
    return sprite.getPosition();
}

void Fox::updateAnimation(float deltaTime) {
    static float animationTimer = 0.0f;
    animationTimer += deltaTime;

    if (animationTimer >= 0.1f) {
        int left = sprite.getTextureRect().left + 32;
        if (left >= 128) left = 32; // Обновление кадров анимации (32, 64, 96, 128 пикселей)
        sprite.setTextureRect(sf::IntRect(left, 0, 32, 32));
        animationTimer = 0.0f;
    }
}
