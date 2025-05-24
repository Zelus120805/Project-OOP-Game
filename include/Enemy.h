#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.h"

class Enemy {
protected:
    float dx, dy;
    sf::FloatRect rect;
    sf::Sprite _enemySprite;
    float currentFrame;
    int maxFrame = 4; // số frame slime có trong 1 animation hàng
    bool life;
    bool _onGround;
    float _hp;
    const int FRAME_WIDTH = 16;
    const int FRAME_HEIGHT = 16;

public:
    Enemy();
    virtual ~Enemy();
public:
    virtual void setEnemy(int x, int y) = 0;
    virtual void updateEnemy(float time, const std::vector<std::string>& tileMap) = 0;
    virtual void Collision(bool checkVertical, const std::vector<std::string>& tileMap);
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
    virtual void takeDamage(float damage);

};

class SlimeEnemy : public Enemy {
private:
    sf::Texture _enemyTexture;
    float _enemySpeed;
    bool _isMovingRight;
    bool _isMovingLeft;
public:
    void setEnemy(int x, int y) override;
    void updateEnemy(float time, const std::vector<std::string>& tileMap) override;
};

#endif // _ENEMY_H_