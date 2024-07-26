#pragma once

#include <SFML/Graphics.hpp>
#include "Grid.h"

class Game {
public:
    Game(int rows, int cols, int cellSize);
    ~Game();
    void run();

private:
    int rows;
    int cols;
    int cellSize;
    sf::RenderWindow window;
    sf::Texture mainTexture;
    sf::Texture topTexture;
    Grid* grid;
    void loadTextures();
    void processEvents();
    void update();
    void render();
};
