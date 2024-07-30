#include "Game.h"
#include <iostream>
#include "Pathfinding.h"

Game::Game(int rows, int cols, int cellSize)
    : rows(rows), cols(cols), cellSize(cellSize),
    window(sf::VideoMode(cols* cellSize, rows* cellSize), "Foxes and Chickens Game"),
    selectedChicken(nullptr), selectedFox(nullptr), chickensTurn(true), moveCount(0), showTurnMessage(true) {

    loadTextures();
    grid = new Grid(rows, cols, cellSize); // Initialize the grid after loading textures

    // Set textures for cells
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (i < 3 && j < 3) {
                grid->setTexture(i, j, &topTexture); // Top 3x3 cells
            }
            else {
                grid->setTexture(i, j, &mainTexture); // Other cells
            }
        }
    }

    // Initialize chickens
    for (int i = 0; i < 20; ++i) {
        chickens.push_back(Chicken(i % cols * cellSize + cellSize / 2, i / cols * cellSize + cellSize / 2, &chickenTexture));
    }

    // Initialize foxes
    for (int i = 0; i < 2; ++i) {
        foxes.push_back(Fox((cols - 1 - i) * cellSize + cellSize / 2, (rows - 1) * cellSize + cellSize / 2, &foxTexture));
    }

    // Load font
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }

    // Configure turn message
    turnMessage.setFont(font);
    turnMessage.setCharacterSize(24);
    turnMessage.setFillColor(sf::Color::Black);
    turnMessage.setStyle(sf::Text::Bold);
    turnMessage.setString("Chicken's Turn");

    // Configure turn message box
    turnMessageBox.setSize(sf::Vector2f(300, 50));
    turnMessageBox.setFillColor(sf::Color(255, 255, 255, 200)); // Semi-transparent white
    turnMessageBox.setOutlineColor(sf::Color::Black);
    turnMessageBox.setOutlineThickness(2);
    turnMessageBox.setPosition((window.getSize().x - turnMessageBox.getSize().x) / 2,
        (window.getSize().y - turnMessageBox.getSize().y) / 2);
    turnMessage.setPosition(turnMessageBox.getPosition().x + 10, turnMessageBox.getPosition().y + 10);
}

Game::~Game() {
    delete grid; // Free memory allocated for the grid
}

void Game::loadTextures() {
    if (!mainTexture.loadFromFile("textures/grass-texture1.png")) { // Relative path
        std::cerr << "Error loading main texture" << std::endl;
    }
    if (!topTexture.loadFromFile("textures/grass-texture2.png")) { // Relative path
        std::cerr << "Error loading top texture" << std::endl;
    }
    if (!chickenTexture.loadFromFile("textures/chicken.png")) { // Relative path
        std::cerr << "Error loading chicken texture" << std::endl;
    }
    if (!foxTexture.loadFromFile("textures/fox3.png")) { // Relative path
        std::cerr << "Error loading fox texture" << std::endl;
    }
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        processEvents();
        float deltaTime = clock.restart().asSeconds();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            handleMouseClick(event.mouseButton.x, event.mouseButton.y);
        }
    }
}

void Game::update(float deltaTime) {
    for (auto& chicken : chickens) {
        chicken.update(deltaTime);
    }

    for (auto& fox : foxes) {
        fox.update(deltaTime);
    }


    // Check if the turn message should be hidden
    if (showTurnMessage && turnMessageClock.getElapsedTime() >= turnMessageDuration) {
        showTurnMessage = false;
    }
}

void Game::render() {
    window.clear(sf::Color::White);
    grid->draw(window);

    // Draw possible moves
    for (const auto& highlight : possibleMoveHighlights) {
        window.draw(highlight);
    }

    for (const auto& chicken : chickens) {
        chicken.draw(window);
    }

    for (const auto& fox : foxes) {
        fox.draw(window);
    }

    // Draw turn message
    if (showTurnMessage) {
        window.draw(turnMessageBox);
        window.draw(turnMessage);
    }

    window.display();
}

void Game::handleMouseClick(int x, int y) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(x, y));

    if (selectedChicken) {
        if (chickensTurn) {
            for (const auto& highlight : possibleMoveHighlights) {
                if (highlight.getGlobalBounds().contains(mousePos)) {
                    selectedChicken->startMoving(highlight.getPosition() + sf::Vector2f(cellSize / 2, cellSize / 2));
                    movedChickens.push_back(selectedChicken); // Add to moved chickens
                    selectedChicken = nullptr;
                    possibleMoveHighlights.clear();
                    if (allChickensMoved()) {
                        switchTurn();
                    }
                    return;
                }
            }
            selectedChicken = nullptr;
            possibleMoveHighlights.clear();
        }
    }
    else if (selectedFox) {
        if (!chickensTurn) {
            for (const auto& highlight : possibleMoveHighlights) {
                if (highlight.getGlobalBounds().contains(mousePos)) {
                    sf::Vector2f dest = highlight.getPosition() + sf::Vector2f(cellSize / 2, cellSize / 2);
                    selectedFox->startMoving(dest);
                    movedFoxes.push_back(selectedFox); // Add to moved foxes
                    selectedFox = nullptr;
                    possibleMoveHighlights.clear();
                    if (allFoxesMoved()) {
                        switchTurn();
                    }
                    return;
                }
            }
            selectedFox = nullptr;
            possibleMoveHighlights.clear();
        }
    }
    else {
        for (auto& chicken : chickens) {
            if (!chicken.isMoving() && chicken.getBounds().contains(mousePos) && chickensTurn && std::find(movedChickens.begin(), movedChickens.end(), &chicken) == movedChickens.end()) {
                selectedChicken = &chicken;
                possibleMoveHighlights.clear();

                sf::Vector2f pos = chicken.getPosition();
                sf::Vector2f moves[] = {
                    sf::Vector2f(pos.x - cellSize, pos.y), // left
                    sf::Vector2f(pos.x + cellSize, pos.y), // right
                    sf::Vector2f(pos.x, pos.y + cellSize)  // down
                };
                for (const auto& move : moves) {
                    if (move.x >= 0 && move.x < cols * cellSize && move
                        .y < rows * cellSize && !isCellOccupied(move)) {
                        sf::RectangleShape highlight(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
                        highlight.setFillColor(sf::Color(0, 255, 0, 128));
                        highlight.setPosition(move.x - cellSize / 2, move.y - cellSize / 2);
                        possibleMoveHighlights.push_back(highlight);
                    }
                }
                return;
            }
        }

        for (auto& fox : foxes) {
            if (!fox.isMoving() && fox.getBounds().contains(mousePos) && !chickensTurn && std::find(movedFoxes.begin(), movedFoxes.end(), &fox) == movedFoxes.end()) {
                selectedFox = &fox;
                possibleMoveHighlights.clear();

                sf::Vector2f pos = fox.getPosition();
                sf::Vector2f moves[] = {
                    sf::Vector2f(pos.x - cellSize, pos.y), // left
                    sf::Vector2f(pos.x + cellSize, pos.y), // right
                    sf::Vector2f(pos.x, pos.y + cellSize), // down
                    sf::Vector2f(pos.x, pos.y - cellSize)  // up
                };
                for (const auto& move : moves) {
                    if (move.x >= 0 && move.x < cols * cellSize && move.y >= 0 && move.y < rows * cellSize && !isCellOccupied(move)) {
                        sf::RectangleShape highlight(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
                        highlight.setFillColor(sf::Color(0, 255, 0, 128)); // Green highlight
                        highlight.setPosition(move.x - cellSize / 2, move.y - cellSize / 2);
                        possibleMoveHighlights.push_back(highlight);
                    }
                }
                return;
            }
        }
    }
}

void Game::switchTurn() {
    chickensTurn = !chickensTurn;
    moveCount = 0;
    movedChickens.clear();
    movedFoxes.clear();
    showTurnMessage = true;
    turnMessageClock.restart();

    if (chickensTurn) {
        turnMessage.setString("Chicken's Turn");
    }
    else {
        turnMessage.setString("Fox's Turn");
    }

    // Center the turn message
    turnMessageBox.setPosition((window.getSize().x - turnMessageBox.getSize().x) / 2,
        (window.getSize().y - turnMessageBox.getSize().y) / 2);
    turnMessage.setPosition(turnMessageBox.getPosition().x + 10, turnMessageBox.getPosition().y + 10);
}

bool Game::allChickensMoved() const {
    return movedChickens.size() == chickens.size();
}

bool Game::allFoxesMoved() const {
    return movedFoxes.size() == foxes.size();
}

bool Game::isCellOccupied(const sf::Vector2f& position) const {
    for (const auto& chicken : chickens) {
        if (chicken.getBounds().contains(position)) {
            return true;
        }
    }
    for (const auto& fox : foxes) {
        if (fox.getBounds().contains(position)) {
            return true;
        }
    }
    return false;
}
