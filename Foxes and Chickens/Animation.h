#pragma once

#include <SFML/Graphics.hpp>

class Animation {
public:
    Animation(sf::Texture* texture, int frameWidth, int frameHeight, int frameCount, float duration)
        : texture(texture), frameWidth(frameWidth), frameHeight(frameHeight), frameCount(frameCount), duration(duration),
        currentFrame(0), elapsedTime(0), mode(Mode::Idle) {
        // Установим начальный кадр
        updateSprite();
    }

    void update(float deltaTime) {
        if (mode == Mode::Moving) {
            elapsedTime += deltaTime;
            if (elapsedTime >= duration) {
                elapsedTime -= duration;
                currentFrame = (currentFrame + 1) % (frameCount - 1); // Исключаем первый кадр (стояния)
            }
        }
        else {
            currentFrame = 0; // Показываем кадр стояния
        }
        updateSprite();
    }

    void setMode(bool isMoving) {
        mode = isMoving ? Mode::Moving : Mode::Idle;
    }

    void applyToSprite(sf::Sprite& sprite) {
        sprite.setTexture(*texture);
        sprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
    }

private:
    enum class Mode { Idle, Moving };

    sf::Texture* texture;
    int frameWidth;
    int frameHeight;
    int frameCount;
    float duration;
    int currentFrame;
    float elapsedTime;
    Mode mode;

    void updateSprite() {
        if (mode == Mode::Moving && currentFrame == 0) {
            currentFrame = 1; // Переключаемся на первый кадр анимации движения
        }
    }
};
