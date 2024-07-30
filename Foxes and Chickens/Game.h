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
    bool isCellOccupied(const sf::Vector2f& position) const;

private:
    void loadTextures();
    void processEvents();
    void update(float deltaTime);
    void render();
    void handleMouseClick(int x, int y);
    void switchTurn();
    bool allChickensMoved() const;
    bool allFoxesMoved() const;

    sf::RenderWindow window;
    int rows, cols, cellSize;
    Grid* grid;
    sf::Texture mainTexture, topTexture, chickenTexture, foxTexture;
    std::vector<Chicken> chickens;
    std::vector<Fox> foxes;
    Chicken* selectedChicken;
    Fox* selectedFox;
    std::vector<sf::RectangleShape> possibleMoveHighlights;
    bool chickensTurn;
    int moveCount;

    // For tracking moved chickens and foxes
    std::vector<Chicken*> movedChickens;
    std::vector<Fox*> movedFoxes;

    // Variables for turn message display
    sf::Font font;
    sf::Text turnMessage;
    sf::RectangleShape turnMessageBox;
    bool showTurnMessage;
    sf::Clock turnMessageClock;
    sf::Time turnMessageDuration = sf::seconds(2);
};
