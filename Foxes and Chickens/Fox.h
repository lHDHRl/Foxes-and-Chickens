#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Chicken.h"

class Fox {
public:
    Fox(float x, float y, sf::Texture* texture);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    void startMoving(const sf::Vector2f& dest);
    bool isMoving() const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;
    bool canEatChicken(const sf::Vector2f& movePos, const std::vector<Chicken>& chickens, const std::vector<Fox>& foxes);
    void eatChicken(const sf::Vector2f& movePos, std::vector<Chicken>& chickens);
    void setPosition(const sf::Vector2f& position);

private:
    sf::Sprite sprite;
    sf::Vector2f destination;
    float speed;
    bool moving;
    float eatRange;
    static constexpr int cellSize = 64; // Ensure cellSize is defined and constant
    void updateAnimation(float deltaTime);
};
