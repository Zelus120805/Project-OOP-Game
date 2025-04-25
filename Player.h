#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Map.h"

using namespace sf;

class Player {
private:
    SoundBuffer buffer;
    Sound sound;

    float dx, dy;
    FloatRect rect;
    bool onGround;
    Sprite sprite;
    float currentFrame;

    bool isHit = false;
    int flashCount = 0;
    Clock hitClock;
private:
    void initSound() {
        if (!buffer.loadFromFile("Jump.ogg")) {
            std::cerr << "Missing Jump.ogg\n";
        }
        sound.setBuffer(buffer);
    }
public:
    FloatRect getRect() { return rect; }

    Sprite& getSprite() { return sprite; }

    FloatRect getRect() const { return rect; }

    float getDY() const { return dy; }

    void setDY(float value) { dy = value; }

    void setIsHit(bool value) { isHit = value; }

    Clock getHitClock() { return hitClock; };

    void setFlashCount(int value) { flashCount = value; }
public:
    Player() {
       initSound();
    } // constructor mặc định

    Player(Texture& texture, float x, float y) {
        sprite.setTexture(texture);
        rect = FloatRect(x, y, 16, 16);
        dx = dy = 0.1;
        currentFrame = 0;
        initSound();
    }

    void setPlayer(Texture& texture, float x, float y) {
        sprite.setTexture(texture);
        rect = FloatRect(x, y, 16, 16);
        dx = dy = 0.1;
        currentFrame = 0;
    }

    ~Player() { }

    void controlPlayer() {
        if (Keyboard::isKeyPressed(Keyboard::A))
            dx = -0.1;
        if (Keyboard::isKeyPressed(Keyboard::D))
            dx = 0.1;
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            if (onGround) {
                dy = -0.3;
                onGround = false;
                sound.play();
            }
        }
    }

    void update(float time, const std::vector<std::string>& tileMap) {
        rect.left += dx * time;
        Collision(0, tileMap);

        if (!onGround) dy += 0.0005f * time;
        rect.top += dy * time;
        onGround = false;
        Collision(1, tileMap);

        currentFrame += time * 0.005f;
        if (currentFrame > 3) currentFrame -= 3;

        if (dx > 0) sprite.setTextureRect(IntRect(112 + 31 * int(currentFrame), 144, 16, 16));
        if (dx < 0) sprite.setTextureRect(IntRect(112 + 31 * int(currentFrame) + 16, 144, -16, 16));

        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
        dx = 0;

        if (isHit) {
            if (hitClock.getElapsedTime().asMilliseconds() > 200) { // mỗi 100ms đổi màu
                if (flashCount % 2 == 0)
                    sprite.setColor(Color::Red);
                else
                    sprite.setColor(Color::White);
        
                flashCount++;
                hitClock.restart();
        
                if (flashCount >= 6) { // 3 lần nhấp nháy
                    isHit = false;
                    flashCount = 0;
                    sprite.setColor(Color::White);
                }
            }
        }
    }

    void Collision(int num, const std::vector<std::string>& tileMap) {
        for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++)
            for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
                if (tileMap[i][j] == 'P' || tileMap[i][j] == 'k' || tileMap[i][j] == '0' || tileMap[i][j] == 'r' || tileMap[i][j] == 't') {
                    if (dy > 0 && num == 1) { rect.top = i * 16 - rect.height; dy = 0; onGround = true; }
                    if (dy < 0 && num == 1) { rect.top = i * 16 + 16; dy = 0; }
                    if (dx > 0 && num == 0) { rect.left = j * 16 - rect.width; }
                    if (dx < 0 && num == 0) { rect.left = j * 16 + 16; }
                }
            }
    }
};

#endif // PLAYER_H
