#include "Game.h";

int main() {
    Game game(10, 10, 64); //N,M,Q Создание игры с сеткой 10x10 и размером ячейки 64 пикселя
    game.run();           // Запуск игрового цикла
    return 0;
}
