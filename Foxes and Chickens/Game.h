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
    void startGame(); // Method to start the game
    bool checkChickenVictory(); // Метод для проверки победы куриц
    void displayVictoryMessage(); // Метод для отображения сообщения о победе
    bool isCellFreeBehind(const sf::Vector2f& chickenPos, const sf::Vector2f& foxPos) const;
private:
    sf::Vector2f getPositionAfterEating(const sf::Vector2f& chickenPos, const sf::Vector2f& foxPos) const;
    // Количество куриц и размер поля
    const int numChickens;
    const int centerRow;
    const int centerCol;
    const int spreadRange;
    void handleEating(Fox* fox, int chickenId);
    bool isFoxNearby(const sf::Vector2f& dest) const;
    bool isChickenNearby(const sf::Vector2f& move) const;
    void handleMouseClick(int x, int y);
    bool handleSkipTurnButton(const sf::Vector2f& mousePos);
    bool handleSelectedChicken(const sf::Vector2f& mousePos);
    bool handleSelectedFox(const sf::Vector2f& mousePos);
    bool handleFoxEating(const sf::Vector2f& dest);
    void handleSelection(const sf::Vector2f& mousePos);
    bool handleChickenSelection(const sf::Vector2f& mousePos);
    bool handleFoxSelection(const sf::Vector2f& mousePos);
    void highlightChickenMoves(const Chicken& chicken);
    void highlightFoxMoves(const Fox& fox);
    void switchTurn();
    bool allChickensMoved() const;
    bool allFoxesMoved() const;
    bool isVictoryConditionMet() const; // Метод для проверки условия победы
    bool allCellsOccupied(const sf::Vector2i& topLeft, int size) const; // Метод для проверки занятости всех клеток
    // Переменная для отображения сообщения о победе
    bool victoryMessageDisplayed;
    sf::Text victoryMessage;
    sf::RenderWindow window;
    int rows, cols, cellSize;
    Grid* grid;
    sf::Texture mainTexture, topTexture, chickenTexture, foxTexture;
    sf::RectangleShape skipTurnButton;
    sf::Text skipTurnText;
    sf::Font font;
    sf::Text turnMessage;
    sf::RectangleShape turnMessageBox;
    sf::Text skipTurnMessage;
    sf::RectangleShape skipTurnMessageBox;

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
    bool showTurnMessage;
    sf::Clock turnMessageClock;
    sf::Time turnMessageDuration = sf::seconds(2);

    // Flag to track if the game has started
    bool gameStarted;
    bool showSkipTurnMessage;
    sf::Clock skipTurnMessageClock;
    sf::Time skipTurnMessageDuration = sf::seconds(2);
    sf::Texture skipTurnMessageTexture;

    void loadTextures();
    void processEvents();
    void update(float deltaTime);
    void render();
};
