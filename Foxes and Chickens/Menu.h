#pragma once

#include <SFML/Graphics.hpp>

class Menu {
public:
    Menu(sf::RenderWindow& window);
    bool run(); // Return true if "Play" button was clicked

private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text title;
    sf::Text playButton;
    sf::Text rulesButton;

    // For the rules window
    sf::RectangleShape rulesWindow;
    sf::Text rulesText;
    sf::Text closeButton;
    bool showRules;

    void loadResources();
    void processEvents(bool& playClicked);
    void render();
    void renderRules();
};