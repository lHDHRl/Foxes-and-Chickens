#pragma once

#include <SFML/Graphics.hpp>
#include "Grid.h"

class Game {
public:
    Game(int rows, int cols, int cellSize);
    void run();

private:
    int rows;
    int cols;
    int cellSize;
    sf::RenderWindow window;
    sf::Texture texture;
    Grid grid;
    void loadTexture();
    void processEvents();
    void update();
    void render();
};