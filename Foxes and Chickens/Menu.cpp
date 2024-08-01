#include "Menu.h"
#include <iostream>

Menu::Menu(sf::RenderWindow& window) : window(window), showRules(false) {
    loadResources();

    title.setFont(font);
    title.setString(L"Куры и Лисы");
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::Black);
    title.setPosition((window.getSize().x - title.getGlobalBounds().width) / 2, 50);

    playButton.setFont(font);
    playButton.setString(L"Играть");
    playButton.setCharacterSize(30);
    playButton.setFillColor(sf::Color::Black);
    playButton.setPosition((window.getSize().x - playButton.getGlobalBounds().width) / 2, window.getSize().y / 2);

    rulesButton.setFont(font);
    rulesButton.setString(L"Правила");
    rulesButton.setCharacterSize(30);
    rulesButton.setFillColor(sf::Color::Black);
    rulesButton.setPosition((window.getSize().x - rulesButton.getGlobalBounds().width) / 2, window.getSize().y / 2 + 50);

    // Initialize rules window
    rulesWindow.setSize(sf::Vector2f(600, 400));
    rulesWindow.setFillColor(sf::Color(255, 255, 255, 255));
    rulesWindow.setOutlineColor(sf::Color::Black);
    rulesWindow.setOutlineThickness(2);
    rulesWindow.setPosition((window.getSize().x - rulesWindow.getSize().x) / 2, (window.getSize().y - rulesWindow.getSize().y) / 2);

    rulesText.setFont(font);
    rulesText.setCharacterSize(16);
    rulesText.setFillColor(sf::Color::Black);
    rulesText.setString(L"Правила игры\n\n1.На поле находятся 2 лисы, 20 кур.\n\n2.Вы управляете курами, которые\nперемещаются на один шаг влево,\nвверх, вправо. Лисы, управляемые компьютером,\nперемещаются по тем же направлениям,\nплюс могут сделать шаг назад.\n\n3.Ваша задача : собрать из кур квадрат\n3 на 3 в верхней части поля.Если лисы\nсъедают 12 кур – вы проиграли.");
    rulesText.setPosition(rulesWindow.getPosition().x + 20, rulesWindow.getPosition().y + 20);

    closeButton.setFont(font);
    closeButton.setString(L"Закрыть");
    closeButton.setCharacterSize(16);
    closeButton.setFillColor(sf::Color::Black);
    closeButton.setPosition((rulesWindow.getPosition().x + rulesWindow.getSize().x - 100) / 2, rulesWindow.getPosition().y + rulesWindow.getSize().y - 50);
}

void Menu::loadResources() {
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }
}

void Menu::processEvents(bool& playClicked) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            if (showRules) {
                if (closeButton.getGlobalBounds().contains(mousePos)) {
                    showRules = false;
                }
            }
            else {
                if (playButton.getGlobalBounds().contains(mousePos)) {
                    playClicked = true;
                    return;
                }
                else if (rulesButton.getGlobalBounds().contains(mousePos)) {
                    showRules = true;
                }
            }
        }
    }
}

void Menu::render() {
    window.clear(sf::Color::White);
    window.draw(title);
    window.draw(playButton);
    window.draw(rulesButton);

    if (showRules) {
        renderRules();
    }

    window.display();
}

void Menu::renderRules() {
    window.draw(rulesWindow);
    window.draw(rulesText);
    window.draw(closeButton);
}

bool Menu::run() {
    bool playClicked = false;
    while (window.isOpen() && !playClicked) {
        processEvents(playClicked);
        render();
    }
    return playClicked;
}
