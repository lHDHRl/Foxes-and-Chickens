#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Grid.h"
#include "Chicken.h"
#include "Fox.h"

class Game {
public:
    Game(int rows, int cols, int cellSize);
    ~Game();
    void run();

private:
    void loadTextures();
    void processEvents();
    void update(float deltaTime);
    void render();
    void handleMouseClick(int x, int y);
    bool isCellOccupied(const sf::Vector2f& position) const;

    sf::RenderWindow window;
    int rows, cols, cellSize;
    Grid* grid;
    sf::Texture mainTexture, topTexture, chickenTexture, foxTexture;
    std::vector<Chicken> chickens;
    std::vector<Fox> foxes;
    Chicken* selectedChicken;
    Fox* selectedFox;
    std::vector<sf::RectangleShape> possibleMoveHighlights;
};
