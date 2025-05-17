#ifndef _WEAPON_H_
#define _WEAPON_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Weapon {
protected:
    sf::Texture _texture;
    sf::Sprite _tile;
    sf::FloatRect _rect;
    float _speed;
    bool _active;

    float _damage;
public:
    sf::Sprite getSprite() const;
    sf::Vector2f getPosition() const;
    sf::FloatRect getRect() const;
    float getDamage() const;
public:
    virtual ~Weapon() = default;
public:
    void update(float time, const std::vector<std::string>& currentMap);
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

    void Shoot(float x, float y, bool goingRight);
};

#endif // _WEAPON_H_