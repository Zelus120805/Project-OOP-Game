#ifndef _WEAPON_H_
#define _WEAPON_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <Player.h>
#include <vector>
#include <string>

// class BulletDirection
enum class BulletDirection {
    Left,
    Right,
    Up
};

class Player;

class Weapon {
protected:
    sf::Texture _texture;
    sf::Sprite _tile;
    sf::FloatRect _rect;
    
    bool _active;

    float _damage;

    BulletDirection _direction;

    sf::Clock _shootCooldown;
public:
    sf::Sprite getSprite() const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getRect() const;
    float getDamage() const;
public:
    virtual ~Weapon() = default;
public:
    virtual void update(float time, const std::vector<std::string>& currentMap) = 0;
    virtual void attack(Player& player) = 0;
    void collision(const std::vector<std::string>& tileMap);

    // Getters & utility
    bool isActive() const;
    void deactivate();
    void setActive(bool value);
};

class Gun : public Weapon {
private:
    float _speed;
public:
    Gun();
    ~Gun() override { }

    void attack(Player& player) override;
    void update(float time, const std::vector<std::string>& currentMap) override;
    void Shoot(float x, float y, BulletDirection dir);
};

#endif // _WEAPON_H_