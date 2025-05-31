#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Map.h"
#include "Player.h"
#include "MyException.h"

enum class EnemyType {
    Slime,  
};

inline float GetEnemyDamage(EnemyType type) {
    switch (type) {
        case EnemyType::Slime:
            return 25.f; // Sát thương của Slime
        default:
            return 0.f;
    }
}

class Enemy {
protected:
    float dx, dy;
    sf::FloatRect rect;
    sf::Sprite _enemySprite;
    int maxFrame = 4; // số frame slime có trong 1 animation hàng
    bool life;
    bool _onGround;
    float _hp;
    const int FRAME_WIDTH = 16;
    const int FRAME_HEIGHT = 16;

    // Vi tri
    int _xPos;
    int _yPos; // Vị trí ban đầu của Enemy

    // Các biến death
    bool _isDying = false; // SlimeEnemy đang trong trạng thái dying
    bool _isDeadCompletely = false; // SlimeEnemy đã chết hoàn toàn

    float _damage;
public:
    Enemy();
    virtual ~Enemy();
public:
    virtual void setEnemy() = 0;
    virtual void updateEnemy(float time, const std::vector<std::string>& tileMap, Player& player) = 0;
    virtual void Collision(bool checkVertical, const std::vector<std::string>& tileMap);
public:
    // Getters
    sf::FloatRect getRect() const;
    sf::Sprite getSprite() const;
    bool isAlive() const;
    float getDX() const;
    float getDamage() const;
    int getXPos() const;
    int getYPos() const;
public:
    // Setters
    void setDX(float value);
    virtual void takeDamage(float damage);
    virtual void attack(float playerX) = 0;
    virtual void die() = 0;
    virtual bool isCompleteDie() const = 0;
};

class SlimeEnemy : public Enemy {
private:
    sf::Texture _enemyTexture;
    std::vector<sf::IntRect> _idleFrames;
    std::vector<sf::IntRect> _walkFrames;
    std::vector<sf::IntRect> _attackFrames;
    std::vector<sf::IntRect> _deathFrames;
    float _enemySpeed;
    bool _isMovingRight;
    bool _isMovingLeft;

    float _currentFrame;
    float _frameSpeed;

    // Các biến attack
    bool _isAttacking;
    float _attackDuration;
    float _attackElapsed;
    float _detectionRange = 35.f;  // Phát hiện trong 35px

    float _deathFrame = 0.f;
    float _deathFrameSpeed = 2.f;
    float _deathDuration = 1.f; // tổng thời gian chết
    float _deathElapsed = 0.f;
public:
    bool isAttacking() const;
    bool isCompleteDie() const override;
public:
    SlimeEnemy(int x, int y);
    void setEnemy() override;
    void updateEnemy(float time, const std::vector<std::string>& tileMap, Player& player) override;
    void attack(float playerX) override;
    void die() override;
    void takeDamage(float damage) override;
};

#endif // _ENEMY_H_