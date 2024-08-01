#include "Fox.h"
#include <cmath>
#include <algorithm>
#include <random>

Fox::Fox(float x, float y, sf::Texture* texture)
    : destination(x, y), speed(100.0f), moving(false), eatRange(32.0f) {
    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
    sprite.setPosition(x, y);
    sprite.setOrigin(16, 16); // Centering the sprite
    sprite.setScale(3.0f, 3.0f); // Scaling the fox sprite
}

void Fox::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

void Fox::startMoving(const sf::Vector2f& dest) {
    destination = dest;
    moving = true;
    sprite.setTextureRect(sf::IntRect(32, 0, 32, 32)); // Start movement animation
}

void Fox::update(float deltaTime) {
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
            sprite.setTextureRect(sf::IntRect(0, 0, 32, 32)); // Stop movement animation
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
        if (left >= 96) left = 32; // Animation frames (32, 64, 96, 128 frames)
        sprite.setTextureRect(sf::IntRect(left, 0, 32, 32));
        animationTimer = 0.0f;
    }
}

bool Fox::canEatChicken(const sf::Vector2f& movePos, const std::vector<Chicken>& chickens, const std::vector<Fox>& foxes) {
    auto it = std::find_if(chickens.begin(), chickens.end(), [movePos](const Chicken& chicken) {
        return chicken.getBounds().contains(movePos);
        });

    if (it == chickens.end()) {
        return false; // Нет курицы в movePos
    }

    // Проверка, что позиция для прыжка не выходит за границы игрового поля
    sf::Vector2f direction = movePos - getPosition();
    sf::Vector2f jumpPos = getPosition(); // Не используем jumpPos

    if (jumpPos.x < 0 || jumpPos.x >= 8 * cellSize || jumpPos.y < 0 || jumpPos.y >= 8 * cellSize) {
        return false; // Позиция для прыжка за границами
    }

    // Проверка на занятость позиции другими лисами
    auto foxIt = std::find_if(foxes.begin(), foxes.end(), [jumpPos](const Fox& fox) {
        return fox.getBounds().contains(jumpPos);
        });

    return foxIt == foxes.end(); // Вернуть true, если позиция не занята другой лисой
}

void Fox::eatChicken(const sf::Vector2f& movePos, std::vector<Chicken>& chickens) {
    // Удаляем курицу, если она есть в клетке
    auto it = std::remove_if(chickens.begin(), chickens.end(), [movePos](const Chicken& chicken) {
        return chicken.getBounds().contains(movePos);
        });
    chickens.erase(it, chickens.end());

    // Устанавливаем позицию лисы на позицию съеденной курицы
    setPosition(movePos);
}

void Fox::setPosition(const sf::Vector2f& position) {
    sprite.setPosition(position);
}

