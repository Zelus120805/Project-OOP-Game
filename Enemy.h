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

public:
    Enemy();
    virtual ~Enemy();

    void setEnemy(sf::Texture& image, int x, int y);
    void update(float time, const std::vector<std::string>& tileMap);
    void Collision(const std::vector<std::string>& tileMap);

    // Getters
    sf::FloatRect getRect() const;
    sf::Sprite getSprite() const;
    bool isAlive() const;
    float getDX() const;

    // Setters
    void setDX(float value);
    void setAlive(bool value);
};

#endif // _ENEMY_H_