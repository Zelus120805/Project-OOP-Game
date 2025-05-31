#ifndef _WEAPON_H_
#define _WEAPON_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <Player.h>
#include <vector>
#include <string>

// class BulletDirection
enum class WeaponDirection {
    Left,
    Right,
    Up
};

enum class TypeWeapon {
    Gun,
};

inline float GetSpeedWeapon(TypeWeapon type) {
    switch (type) {
        case TypeWeapon::Gun:
            return 0.2f;
        default:
            return 0.f;
    }
}

inline float GetDamageSpeed(TypeWeapon type) {
    switch (type) {
        case TypeWeapon::Gun:
            return 60.f;
        default:
            return 0.f;
    }
}

inline float GetCoolDownWeapon(TypeWeapon type) {
    switch (type) {
        case TypeWeapon::Gun:
            return 500.f;
        default:
            return 0.f;
    }
}

class Player;

class Weapon {
protected:
    sf::Texture _texture;
    sf::Sprite _tile;
    sf::FloatRect _rect;
    
    bool _active;
    float _damage;
    float _speed;

    WeaponDirection _direction;

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
public:
    Gun();
    ~Gun() override { }

    void attack(Player& player) override;
    void update(float time, const std::vector<std::string>& currentMap) override;
    void Shoot(float x, float y, WeaponDirection dir);
};

#endif // _WEAPON_H_