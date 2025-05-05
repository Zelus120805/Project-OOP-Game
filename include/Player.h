#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Map.h"
#include "Bullet.h"

class Player {
private:
    sf::SoundBuffer buffer;
    sf::Sound sound;

    float dx, dy;
    sf::FloatRect rect;
    bool onGround;
    float currentFrame;

    bool isHit = false;
    int flashCount = 0;
    sf::Clock hitClock;

    sf::Texture _text;
    sf::Sprite _playerSet;

    Bullet _bullet;

    bool _checkRight;

    sf::Sprite _bulletSet;
    std::vector<Bullet> _bullets;
    sf::Clock _shootCooldown;

    float _hp;
private:
    void initSound();
    void isAttacked();
public:
    Player();
    ~Player();
    void setPlayer(float x, float y);
public:
    sf::FloatRect getRect() const;
    sf::Sprite& getPlayerSprite();
    Bullet& getBullet();
    sf::Sprite& getBulletSprite();
    float getDY() const;
    void setDY(float value);
    void setIsHit(bool value);
    sf::Clock getHitClock();
    void setFlashCount(int value);
    bool getIsHit() const;
    std::vector<Bullet>& getBullets();
public:
    void shoot();
    void updateBullets(float time, const std::vector<std::string>& tileMap);
public:
    void controlPlayer(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key up, sf::Keyboard::Key fire);
    void update(float time, const std::vector<std::string>& tileMap, sf::RenderWindow& window);
    void Collision(bool checkVertical, const std::vector<std::string>& tileMap);
};

#endif // _PLAYER_H_