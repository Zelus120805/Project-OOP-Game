#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include "Map.h"
#include "Bullet.h"

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

    Texture _text;
    Sprite _playerSet;

    Bullet _bullet;

    bool _checkRight;

    // Bullet
    Sprite _bulletSet;
    std::vector<Bullet> _bullets; // Nhiều viên đạn
    sf::Clock _shootCooldown;     // Giới hạn tốc độ bắn
private:
    void initSound() {
        if (!buffer.loadFromFile("Sound/Jump.ogg")) {
            std::cerr << "Missing Jump.ogg\n";
        }
        sound.setBuffer(buffer);
    }

    void isAttacked() {
        if (isHit) {
            if (hitClock.getElapsedTime().asMilliseconds() > 200) { // mỗi 100ms đổi màu
                if (flashCount % 2 == 0)
                    _playerSet.setColor(Color::Red);
                else
                _playerSet.setColor(Color::White);
        
                flashCount++;
                hitClock.restart();
        
                if (flashCount >= 6) { // 3 lần nhấp nháy
                    isHit = false;
                    flashCount = 0;
                    _playerSet.setColor(Color::White);
                }
            }
        }
    }
public:
    FloatRect getRect() { return rect; }

    Sprite& getPlayerSprite() { return _playerSet; }

    FloatRect getRect() const { return rect; }

    Bullet& getBullet() { return _bullet; }

    Sprite& getBulletSprite() { return _bulletSet; }

    float getDY() const { return dy; }

    void setDY(float value) { dy = value; }

    void setIsHit(bool value) { isHit = value; }

    Clock getHitClock() { return hitClock; };

    void setFlashCount(int value) { flashCount = value; }

    bool getIsHit() const { return isHit; }

    // Bullet
    const std::vector<Bullet>& getBullets() const { return _bullets; }
public:
    Player() {
       initSound();
    } // constructor mặc định

    ~Player() { }

    void setPlayer(float x, float y) {
        if (!_text.loadFromFile("Player/Contra.png")) {
            std::cerr << "Error loading Contra.png\n";
        }
    
        _playerSet.setTexture(_text);
        _bulletSet.setTexture(_text);
    
        rect = FloatRect(x, y, 25, 35);  // nhân vật 24x32
        dx = dy = 0;                     // ban đầu đứng yên
        currentFrame = 0;
        _checkRight = true; // mặc định là đi phải
    
        _playerSet.setTextureRect(IntRect(24 * 8, 8 - 5, 30, 32 + 4)); // Set đúng frame nhân vật đứng
        _bulletSet.setTextureRect(IntRect(51 * 8 - 2, 8 * 2 + 4, 6, 6)); // Set đúng frame viên đạn
    }
public:
    // Bullet
    void shoot() {
        if (_shootCooldown.getElapsedTime().asMilliseconds() >= 500) {
            Bullet bullet;
            if (_checkRight) {
                bullet.Shoot(rect.left + rect.width / 2 + 12, rect.top + rect.height / 2 - 6, _checkRight);
            } else {
                bullet.Shoot(rect.left - 2, rect.top + rect.height / 2 - 6, _checkRight);
            }
            _bullets.push_back(bullet);
            _shootCooldown.restart();
        }
    }

    void updateBullets(float time, const std::vector<std::string>& tileMap) {
        for (auto& b : _bullets)
            b.update(time, tileMap);
        
        // Xoá đạn đã tắt
        _bullets.erase(
            std::remove_if(_bullets.begin(), _bullets.end(),
                        [](const Bullet& b) { return !b.isActive(); }),
            _bullets.end());
    }
public:
    void controlPlayer(Keyboard::Key left, Keyboard::Key right, Keyboard::Key up, Keyboard::Key fire) {
        if (Keyboard::isKeyPressed(left)) {
            dx = -0.05;
            _checkRight = false;
        }
        if (Keyboard::isKeyPressed(right)) {
            dx = 0.05;
            _checkRight = true;
        }
        if (Keyboard::isKeyPressed(up)) {
            if (onGround) {
                dy = -0.3;
                onGround = false;
                sound.play();
            }
        }
        if (Keyboard::isKeyPressed(fire)) {
            shoot(); // Bắn đạn
        }
    }

    void update(float time, const std::vector<std::string>& tileMap, sf::RenderWindow& window) {
        rect.left += dx * time;
        Collision(false, tileMap);
    
        if (!onGround) 
            dy += 0.0005f * time;
        rect.top += dy * time;
        onGround = false;
        Collision(true, tileMap);
    
        currentFrame += time * 0.005f;
        if (currentFrame > 5)
            currentFrame = 0; // 5 frames chạy
    
        // Animation
        if (dx > 0) { // đi phải
            _playerSet.setTextureRect(IntRect(19 * 8 - 40 * int(currentFrame), 8 - 5, 30, 32 + 4));
        }
        else if (dx < 0) { // đi trái
            _playerSet.setTextureRect(IntRect(19 * 8 - 40 * int(currentFrame) + 30, 8 - 5, -30, 32 + 4));
        }
        else { // đứng yên
            if (_checkRight) 
            _playerSet.setTextureRect(IntRect(24 * 8, 8 - 5, 30, 32 + 4));
            else
            _playerSet.setTextureRect(IntRect(24 * 8 + 30, 8 - 5, -30, 32 + 4));
        }
    
        _playerSet.setPosition(rect.left - offsetX, rect.top - offsetY);
    
        dx = 0; // reset dx sau mỗi frame

        isAttacked(); // kiểm tra va chạm với enemy
    }    
    
    void Collision(bool checkVertical, const std::vector<std::string>& tileMap) {
        for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++)
            for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
                if (   tileMap[i][j] == 'P' 
                    || tileMap[i][j] == 'k' 
                    || tileMap[i][j] == '0' 
                    || tileMap[i][j] == 'r' 
                    || tileMap[i][j] == 't') 
                {
                    if (dy > 0 && checkVertical) { 
                        rect.top = i * 16 - rect.height; 
                        dy = 0; 
                        onGround = true; 
                    }

                    if (dy < 0 && checkVertical) { 
                        rect.top = i * 16 + 16; 
                        dy = 0; 
                    }

                    if (dx > 0 && !checkVertical)
                        rect.left = j * 16 - rect.width;

                    if (dx < 0 && !checkVertical) 
                        rect.left = j * 16 + 16;
                }
            }
    }
};

#endif // PLAYER_H