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
    float currentFrame;

    bool isHit = false;
    int flashCount = 0;
    Clock hitClock;

    Sprite _tile;
    Texture _tileSet;

    bool _checkRight;
private:
    void initSound() {
        if (!buffer.loadFromFile("Sound/Jump.ogg")) {
            std::cerr << "Missing Jump.ogg\n";
        }
        sound.setBuffer(buffer);
    }
public:
    FloatRect getRect() { return rect; }

    Sprite& getSprite() { return _tile; }

    FloatRect getRect() const { return rect; }

    float getDY() const { return dy; }

    void setDY(float value) { dy = value; }

    void setIsHit(bool value) { isHit = value; }

    Clock getHitClock() { return hitClock; };

    void setFlashCount(int value) { flashCount = value; }

    bool getIsHit() const { return isHit; }
public:
    Player() {
       initSound();
    } // constructor mặc định

    ~Player() { }

    void setPlayer(float x, float y) {
        if (!_tileSet.loadFromFile("Player/ContraSheet.png")) {
            std::cerr << "Error loading ContraSheet.png\n";
        }
    
        _tile.setTexture(_tileSet);
    
        rect = FloatRect(x, y, 25, 35);  // nhân vật 24x32
        dx = dy = 0;                     // ban đầu đứng yên
        currentFrame = 0;
        _checkRight = true; // mặc định là đi phải
    
        _tile.setTextureRect(IntRect(144, 16, 24, 32 + 4)); // Set đúng frame nhân vật đứng
    }

    void controlPlayer() {
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            dx = -0.1;
            _checkRight = false;
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            dx = 0.1;
            _checkRight = true;
        }
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
        if (currentFrame > 5) currentFrame = 0; // 5 frames chạy
    
        // Animation
        if (dx > 0) { // đi phải
            _tile.setTextureRect(IntRect(144 - 2 + 24 * int(currentFrame), 136 - 4, 24, 32 + 4));
        }
        else if (dx < 0) { // đi trái
            _tile.setTextureRect(IntRect(144 - 2 + 24 * int(currentFrame) + 24, 136 - 4, -24, 32 + 4));
        }
        else { // đứng yên
            if (_checkRight) 
                _tile.setTextureRect(IntRect(144, 16, 24, 32 + 4)); 
            else
                _tile.setTextureRect(IntRect(144 + 24, 16, -24, 32 + 4));
        }
    
        _tile.setPosition(rect.left - offsetX, rect.top - offsetY);
    
        dx = 0; // reset dx sau mỗi frame

        if (isHit) {
            if (hitClock.getElapsedTime().asMilliseconds() > 200) { // mỗi 100ms đổi màu
                if (flashCount % 2 == 0)
                    _tile.setColor(Color::Red);
                else
                    _tile.setColor(Color::White);
        
                flashCount++;
                hitClock.restart();
        
                if (flashCount >= 6) { // 3 lần nhấp nháy
                    isHit = false;
                    flashCount = 0;
                    _tile.setColor(Color::White);
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
