#include "Chicken.h"

Chicken::Chicken(float x, float y, sf::Texture* texture, int id)
    : destination(x, y), speed(100.0f), moving(false), id(id) {
    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    sprite.setPosition(x, y);
    sprite.setOrigin(16, 16); // Центрирование спрайта
    sprite.setScale(1.1f, 1.1f); // Увеличение спрайта на 10%
}

void Chicken::update(float deltaTime) {
    if (moving) {
        sf::Vector2f direction = destination - sprite.getPosition();
        float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance > 1.0f) {
            sf::Vector2f velocity = direction / distance * speed;
            sprite.move(velocity * deltaTime);
        }
        else {
            sprite.setPosition(destination);
            moving = false;
            sprite.setTextureRect(sf::IntRect(0, 0, 32, 32)); // Сброс текстуры к обычному состоянию
        }

        updateAnimation(deltaTime);
    }
}

void Chicken::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void Chicken::startMoving(const sf::Vector2f& dest) {
    destination = dest;
    moving = true;
    sprite.setTextureRect(sf::IntRect(32, 0, 32, 32)); // Изменение текстуры при движении
}

bool Chicken::isMoving() const {
    return moving;
}

sf::FloatRect Chicken::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Chicken::getPosition() const {
    return sprite.getPosition();
}

int Chicken::getId() const {
    return id;
}

void Chicken::updateAnimation(float deltaTime) {
    static float animationTimer = 0.0f;
    animationTimer += deltaTime;

    if (animationTimer >= 0.1f) {
        int left = sprite.getTextureRect().left + 32;
        if (left >= 96) left = 32; // Циклическая анимация
        sprite.setTextureRect(sf::IntRect(left, 0, 32, 32));
        animationTimer = 0.0f;
    }
}
