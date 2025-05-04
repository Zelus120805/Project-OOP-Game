#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <SFML/Graphics.hpp>
#include "Map.h"

class Enemy {
private:
    float dx, dy;
    sf::FloatRect rect;
    sf::Sprite sprite;
    float currentFrame;
    bool life;
    bool _onGround;

    float _hp;
public:
    Enemy();
    virtual ~Enemy();
public:
    void setEnemy(sf::Texture& image, int x, int y);
    void update(float time, const std::vector<std::string>& tileMap);
    void Collision(bool checkVertical, const std::vector<std::string>& tileMap);
public:
    // Getters
    sf::FloatRect getRect() const;
    sf::Sprite getSprite() const;
    bool isAlive() const;
    float getDX() const;
public:
    // Setters
    void setDX(float value);
    void setAlive(bool value);
    void takeDamage(float damage);

};

#endif // _ENEMY_H_