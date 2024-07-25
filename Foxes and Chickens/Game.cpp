#include "Game.h"

Game::Game(int rows, int cols, int cellSize)
    : rows(rows), cols(cols), cellSize(cellSize),
    window(sf::VideoMode(cols* cellSize, rows* cellSize), "Foxes and Chickens Game"),
    grid(rows, cols, cellSize, texture) {
    loadTexture();
}

void Game::loadTexture() {
    if (!texture.loadFromFile("textures/grass_texture1.png")) {
        sf::err() << "Error loading texture" << std::endl;
    }
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update() {
    // Обновление логики игры (если необходимо)
}

void Game::render() {
    window.clear(sf::Color::White);
    grid.draw(window);
    window.display();
}