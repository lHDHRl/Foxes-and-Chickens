#include "Game.h"
#include "Fox.h"
#include "Chicken.h"
#include <iostream>
#include "Menu.h"
#include <map>
#include <algorithm> // Для std::find_if
#include <cstdlib> // Для std::rand и std::srand
#include <ctime>   // Для std::time
#include <unordered_set>
#include <utility> // Для std::pair
#include <unordered_set>
#include <functional> // Для std::hash
// Определение Position как std::pair<int, int>
using Position = std::pair<int, int>;

// Хэш-функция для std::pair<int, int>
struct PositionHash {
    std::size_t operator()(const Position& pos) const {
        auto h1 = std::hash<int>{}(pos.first);
        auto h2 = std::hash<int>{}(pos.second);
        return h1 ^ (h2 << 1); // Комбинирование хэшей
    }
};

// Оператор сравнения для std::pair<int, int>
struct PositionEqual {
    bool operator()(const Position& lhs, const Position& rhs) const {
        return lhs.first == rhs.first && lhs.second == rhs.second;
    }
};


// Конструктор класса Game
Game::Game(int rows, int cols, int cellSize)
    : rows(rows), cols(cols), cellSize(cellSize),
    window(sf::VideoMode(cols* cellSize, rows* cellSize + 100), L"Куры и Лисы"),
    selectedChicken(nullptr), selectedFox(nullptr),
    chickensTurn(true), moveCount(0), showTurnMessage(false), gameStarted(false), showSkipTurnMessage(false),numChickens(20), centerRow(rows / 2), centerCol(cols / 2), spreadRange(3) {

    loadTextures(); // Загрузка текстур
    grid = new Grid(rows, cols, cellSize); // Инициализация сетки

    // Установка текстур для ячеек
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid->setTexture(i, j, (i < 3 && j < 3) ? &topTexture : &mainTexture);
        }
    }





    // Инициализация генератора случайных чисел
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Используем std::unordered_set для уникальных позиций
    std::unordered_set<Position, PositionHash, PositionEqual> occupiedPositions;

    // Инициализация куриц в хаотичном порядке около центра
    for (int i = 0; i < numChickens; ++i) {
        int row, col;

        do {
            // Генерация случайных смещений
            int rowOffset = std::rand() % (2 * spreadRange + 1) - spreadRange;
            int colOffset = std::rand() % (2 * spreadRange + 1) - spreadRange;

            // Вычисление позиции с учетом смещений
            row = centerRow + rowOffset;
            col = centerCol + colOffset;

            // Проверка границ и корректировка значений
            if (row < 0) row = 0;
            if (row >= rows) row = rows - 1;
            if (col < 0) col = 0;
            if (col >= cols) col = cols - 1;

        } while (!occupiedPositions.insert(Position(row, col)).second); // Проверка и вставка в множество

        // Создание курицы в вычисленной позиции
        chickens.emplace_back(col * cellSize + cellSize / 2, row * cellSize + cellSize / 2, &chickenTexture, i);
    }

    // Инициализация лис
    for (int i = 0; i < 2; ++i) {
        foxes.emplace_back((cols - 1 - i) * cellSize + cellSize / 2, (rows - 1) * cellSize + cellSize / 2, &foxTexture);
    }


    // Загрузка шрифта
    if (!font.loadFromFile("DejaVuSans.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }

    // Настройка кнопки пропуска хода
    skipTurnButton.setSize(sf::Vector2f(150, 50));
    skipTurnButton.setFillColor(sf::Color(200, 200, 200));
    skipTurnButton.setOutlineColor(sf::Color::Black);
    skipTurnButton.setOutlineThickness(2);
    skipTurnButton.setPosition((window.getSize().x - skipTurnButton.getSize().x) / 2, window.getSize().y - 75);

    skipTurnText.setFont(font);
    skipTurnText.setString(L"Пропустить\n ход");
    skipTurnText.setCharacterSize(16);
    skipTurnText.setFillColor(sf::Color::Black);
    skipTurnText.setPosition(skipTurnButton.getPosition().x + 10, skipTurnButton.getPosition().y + 10);

    // Настройка сообщения о пропуске хода
    skipTurnMessage.setFont(font);
    skipTurnMessage.setCharacterSize(16);
    skipTurnMessage.setFillColor(sf::Color::Black);
    skipTurnMessage.setStyle(sf::Text::Bold);

    skipTurnMessageBox.setSize(sf::Vector2f(300, 50));
    skipTurnMessageBox.setOutlineColor(sf::Color::Black);
    skipTurnMessageBox.setOutlineThickness(2);

    // Настройка сообщения о текущем ходе
    turnMessage.setFont(font);
    turnMessage.setCharacterSize(16);
    turnMessage.setFillColor(sf::Color::Black);
    turnMessage.setStyle(sf::Text::Bold);
    turnMessage.setString(L"Ход куриц!");

    turnMessageBox.setSize(sf::Vector2f(300, 50));
    turnMessageBox.setFillColor(sf::Color(255, 255, 255, 200));
    turnMessageBox.setOutlineColor(sf::Color::Black);
    turnMessageBox.setOutlineThickness(2);
    turnMessageBox.setPosition((window.getSize().x - turnMessageBox.getSize().x) / 2,
        (window.getSize().y - turnMessageBox.getSize().y) / 2);
    turnMessage.setPosition(turnMessageBox.getPosition().x + 10, turnMessageBox.getPosition().y + 10);
}

// Деструктор класса Game
Game::~Game() {
    delete grid; // Освобождение памяти для сетки
}

// Загрузка текстур
void Game::loadTextures() {
    if (!mainTexture.loadFromFile("textures/grass-texture1.png")) {
        std::cerr << "Error loading main texture" << std::endl;
    }
    if (!topTexture.loadFromFile("textures/grass-texture2.png")) {
        std::cerr << "Error loading top texture" << std::endl;
    }
    if (!chickenTexture.loadFromFile("textures/chicken.png")) {
        std::cerr << "Error loading chicken texture" << std::endl;
    }
    if (!foxTexture.loadFromFile("textures/fox3.png")) {
        std::cerr << "Error loading fox texture" << std::endl;
    }
}

// Основной игровой цикл
void Game::run() {
    Menu menu(window); // Создание меню
    if (menu.run()) { // Если выбрано "Играть"
        startGame(); // Начать игру

        sf::Clock clock;
        while (window.isOpen()) {
            processEvents(); // Обработка событий
            float deltaTime = clock.restart().asSeconds(); // Время с последнего кадра
            update(deltaTime); // Обновление состояния
            render(); // Отрисовка
        }
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

// Начать игру
void Game::startGame() {
    gameStarted = true;
    showTurnMessage = true;
    turnMessage.setString(L"Ход куриц");
    turnMessageClock.restart();

    turnMessageBox.setPosition((window.getSize().x - turnMessageBox.getSize().x) / 2,
        (window.getSize().y - turnMessageBox.getSize().y) / 2);
    turnMessage.setPosition(turnMessageBox.getPosition().x + 10, turnMessageBox.getPosition().y + 10);
}

// Обновление состояния игры
void Game::update(float deltaTime) {
    // Проверка на победу куриц
    if (checkChickenVictory()) {
        displayVictoryMessage(); // Отображение сообщения о победе куриц
        return; // Прекращение выполнения, если курицы победили
    }

    // Обновление состояния куриц
    for (auto& chicken : chickens) {
        chicken.update(deltaTime);
    }

    // Обновление состояния лис
    for (auto& fox : foxes) {
        fox.update(deltaTime);
    }

    // Проверка на скрытие сообщений
    if (showTurnMessage && turnMessageClock.getElapsedTime() >= turnMessageDuration) {
        showTurnMessage = false;
    }

    if (showSkipTurnMessage && skipTurnMessageClock.getElapsedTime() >= skipTurnMessageDuration) {
        showSkipTurnMessage = false;
    }
}

// Отрисовка
void Game::render() {
    window.clear(sf::Color::White); // Очистка окна
    grid->draw(window); // Отрисовка сетки

    // Отрисовка подсветок возможных ходов
    for (const auto& highlight : possibleMoveHighlights) {
        window.draw(highlight);
    }

    // Отрисовка куриц
    for (const auto& chicken : chickens) {
        chicken.draw(window);
    }

    // Отрисовка лис
    for (const auto& fox : foxes) {
        fox.draw(window);
    }

    // Отрисовка сообщений
    if (gameStarted) {
        if (showTurnMessage) {
            window.draw(turnMessageBox);
            window.draw(turnMessage);
        }
        window.draw(skipTurnButton);
        window.draw(skipTurnText);
    }

    if (showSkipTurnMessage) {
        window.draw(skipTurnMessageBox);
        window.draw(skipTurnMessage);
    }

    window.display(); // Обновление экрана
}

void Game::handleMouseClick(int x, int y) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(x, y));

    if (handleSkipTurnButton(mousePos)) return;
    if (handleSelectedChicken(mousePos)) return;
    if (handleSelectedFox(mousePos)) return;

    handleSelection(mousePos);
}

bool Game::handleSkipTurnButton(const sf::Vector2f& mousePos) {
    if (skipTurnButton.getGlobalBounds().contains(mousePos) && chickensTurn) {
        showSkipTurnMessage = true;
        skipTurnMessage.setString(L"Курицы пропустили ход!");
        skipTurnMessageClock.restart();
        switchTurn();
        return true;
    }
    return false;
}

bool Game::handleSelectedChicken(const sf::Vector2f& mousePos) {
    if (selectedChicken && chickensTurn) {
        for (const auto& highlight : possibleMoveHighlights) {
            if (highlight.getGlobalBounds().contains(mousePos)) {
                selectedChicken->startMoving(highlight.getPosition() + sf::Vector2f(cellSize / 2, cellSize / 2));
                movedChickens.push_back(selectedChicken);
                selectedChicken = nullptr;
                possibleMoveHighlights.clear();
                if (allChickensMoved()) {
                    switchTurn();
                }
                return true;
            }
        }
        selectedChicken = nullptr;
        possibleMoveHighlights.clear();
    }
    return false;
}

bool Game::handleSelectedFox(const sf::Vector2f& mousePos) {
    if (selectedFox && !chickensTurn) {
        for (const auto& highlight : possibleMoveHighlights) {
            if (highlight.getGlobalBounds().contains(mousePos)) {
                sf::Vector2f dest = highlight.getPosition() + sf::Vector2f(cellSize / 2, cellSize / 2);

                bool chickenEaten = handleFoxEating(dest);
                if (chickenEaten) return true;

                if (!isFoxNearby(dest)) {
                    selectedFox->startMoving(dest);
                    movedFoxes.push_back(selectedFox);
                    selectedFox = nullptr;
                    possibleMoveHighlights.clear();
                    if (allFoxesMoved()) {
                        switchTurn();
                    }
                }
                return true;
            }
        }
        selectedFox = nullptr;
        possibleMoveHighlights.clear();
    }
    return false;
}

bool Game::handleFoxEating(const sf::Vector2f& dest) {
    bool chickenEaten = false;
    for (auto it = chickens.begin(); it != chickens.end(); ) {
        if (it->getBounds().contains(dest)) {
            int chickenId = it->getId();
            handleEating(selectedFox, chickenId);
            it = chickens.erase(it);
            chickenEaten = true;
        }
        else {
            ++it;
        }
    }
    return chickenEaten;
}

bool Game::isFoxNearby(const sf::Vector2f& dest) const {
    for (const auto& fox : foxes) {
        if (fox.getBounds().contains(dest)) {
            return true;
        }
    }
    return false;
}

void Game::handleSelection(const sf::Vector2f& mousePos) {
    if (!selectedChicken && !selectedFox) {
        if (handleChickenSelection(mousePos)) return;
        if (handleFoxSelection(mousePos)) return;
    }
}

bool Game::handleChickenSelection(const sf::Vector2f& mousePos) {
    for (auto& chicken : chickens) {
        if (!chicken.isMoving() && chicken.getBounds().contains(mousePos) && chickensTurn && std::find(movedChickens.begin(), movedChickens.end(), &chicken) == movedChickens.end()) {
            selectedChicken = &chicken;
            possibleMoveHighlights.clear();
            highlightChickenMoves(chicken);
            return true;
        }
    }
    return false;
}

bool Game::handleFoxSelection(const sf::Vector2f& mousePos) {
    for (auto& fox : foxes) {
        if (!fox.isMoving() && fox.getBounds().contains(mousePos) && !chickensTurn && std::find(movedFoxes.begin(), movedFoxes.end(), &fox) == movedFoxes.end()) {
            selectedFox = &fox;
            possibleMoveHighlights.clear();
            highlightFoxMoves(fox);
            return true;
        }
    }
    return false;
}

void Game::highlightChickenMoves(const Chicken& chicken) {
    sf::Vector2f pos = chicken.getPosition();
    sf::Vector2f moves[] = {
        sf::Vector2f(pos.x - cellSize, pos.y),       // left
        sf::Vector2f(pos.x + cellSize, pos.y),       // right
        sf::Vector2f(pos.x, pos.y - cellSize)        // up (заменили вниз на вверх)
    };

    for (const auto& move : moves) {
        // Проверка, что позиция находится внутри границ поля
        if (move.x >= 0 && move.x < cols * cellSize && move.y >= 0 && move.y < rows * cellSize) {
            // Проверка, что ячейка не занята
            if (!isCellOccupied(move)) {
                sf::RectangleShape highlight(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
                highlight.setFillColor(sf::Color(0, 255, 0, 128));
                highlight.setPosition(move.x - cellSize / 2, move.y - cellSize / 2);
                possibleMoveHighlights.push_back(highlight);
            }
        }
    }
}

void Game::highlightFoxMoves(const Fox& fox) {
    sf::Vector2f pos = fox.getPosition();
    sf::Vector2f moves[] = {
        sf::Vector2f(pos.x - cellSize, pos.y), // left
        sf::Vector2f(pos.x + cellSize, pos.y), // right
        sf::Vector2f(pos.x, pos.y + cellSize), // down
        sf::Vector2f(pos.x, pos.y - cellSize)  // up
    };
    for (const auto& move : moves) {
        if (move.x >= 0 && move.x < cols * cellSize && move.y >= 0 && move.y < rows * cellSize) {
            bool chickenNearby = isChickenNearby(move);
            bool foxNearby = isFoxNearby(move);

            sf::RectangleShape highlight(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
            if (chickenNearby) {
                highlight.setFillColor(sf::Color(255, 0, 0, 128)); // Red highlight for chickens
            }
            else if (!foxNearby && !isCellOccupied(move)) {
                highlight.setFillColor(sf::Color(0, 255, 0, 128)); // Green highlight for empty cells
            }
            else {
                highlight.setFillColor(sf::Color(0, 128, 128, 256));
            }
            highlight.setPosition(move.x - cellSize / 2, move.y - cellSize / 2);
            possibleMoveHighlights.push_back(highlight);
        }
    }
}

bool Game::isChickenNearby(const sf::Vector2f& move) const {
    for (const auto& chicken : chickens) {
        if (chicken.getBounds().contains(move)) {
            return true;
        }
    }
    return false;
}

// Смена хода
void Game::switchTurn() {
    chickensTurn = !chickensTurn;
    moveCount = 0;
    movedChickens.clear();
    movedFoxes.clear();
    showTurnMessage = true;
    turnMessageClock.restart();

    turnMessage.setString(chickensTurn ? L"Ход куриц!" : L"Ход лис!");

    turnMessageBox.setPosition((window.getSize().x - turnMessageBox.getSize().x) / 2,
        (window.getSize().y - turnMessageBox.getSize().y) / 2);
    turnMessage.setPosition(turnMessageBox.getPosition().x + 10, turnMessageBox.getPosition().y + 10);
}

// Проверка, что все курицы перемещены
bool Game::allChickensMoved() const {
    return movedChickens.size() == chickens.size();
}

// Проверка, что все лисы перемещены
bool Game::allFoxesMoved() const {
    return movedFoxes.size() == foxes.size();
}

// Проверка, что ячейка занята
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

void Game::handleEating(Fox* fox, int chickenId) {
    // Создаем временную карту для поиска курицы по идентификатору
    std::map<int, Chicken*> chickenMap;
    for (auto& chicken : chickens) {
        chickenMap[chicken.getId()] = &chicken;
    }

    // Находим курицу по идентификатору
    auto it = chickenMap.find(chickenId);
    if (it != chickenMap.end()) {
        Chicken* chicken = it->second;
        // Устанавливаем позицию лисы на позицию съеденной курицы
        fox->setPosition(chicken->getPosition());

        // Удаляем курицу из вектора
        chickens.erase(std::remove_if(chickens.begin(), chickens.end(),
            [chickenId](const Chicken& c) { return c.getId() == chickenId; }),
            chickens.end());
    }
}
bool Game::checkChickenVictory() {
    return allCellsOccupied(sf::Vector2i(0, 0), 3); // Проверка занятости всех клеток в квадрате 3x3
}

// Метод для отображения сообщения о победе
void Game::displayVictoryMessage() {
    // Логика для отображения сообщения о победе
    turnMessage.setString(L"Куры победили!");
    turnMessage.setPosition(window.getSize().x / 2 - turnMessage.getGlobalBounds().width / 2,
        window.getSize().y / 2 - turnMessage.getGlobalBounds().height / 2);
    showTurnMessage = true;
}

bool Game::isVictoryConditionMet() const {
    // Проверка победы куриц в верхнем левом квадрате 3x3
    sf::Vector2i topLeft(0, 0); // Верхний левый угол квадрата
    int size = 3; // Размер квадрата

    return allCellsOccupied(topLeft, size);
}

bool Game::allCellsOccupied(const sf::Vector2i& topLeft, int size) const {
    // Создание множества для хранения занятых позиций
    std::unordered_set<Position, PositionHash, PositionEqual> occupiedPositions;

    for (const auto& chicken : chickens) {
        // Получение позиции курицы
        sf::Vector2f pos = chicken.getPosition();
        sf::Vector2i cell(
            static_cast<int>(pos.x) / cellSize, // Преобразование x позиции в индекс клетки
            static_cast<int>(pos.y) / cellSize  // Преобразование y позиции в индекс клетки
        );
        occupiedPositions.insert(Position(cell.x, cell.y));
    }

    // Проверка, что все клетки в квадрате заняты
    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            Position cell(topLeft.x + x, topLeft.y + y);
            if (occupiedPositions.find(cell) == occupiedPositions.end()) {
                return false; // Не все клетки заняты
            }
        }
    }
    return true; // Все клетки заняты
}