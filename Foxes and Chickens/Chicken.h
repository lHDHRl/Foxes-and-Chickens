#pragma once

#include <SFML/Graphics.hpp>

class Chicken {
public:
    Chicken(float x, float y, sf::Texture* texture);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    void startMoving(const sf::Vector2f& destination);
    bool isMoving() const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

private:
    sf::Sprite sprite;
    sf::Vector2f destination;
    float speed;
    bool moving;

    void updateAnimation(float deltaTime);
};
