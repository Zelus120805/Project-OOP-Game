#ifndef _BULLET_H_
#define _BULLET_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Bullet {
private:
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
    Bullet();
    virtual ~Bullet();
public:
    void Shoot(float x, float y, bool goingRight);
    void update(float time, const std::vector<std::string>& currentMap);
    void collision(const std::vector<std::string>& tileMap);

    // Getters & utility
    bool isActive() const;
    void deactivate();
    void setActive(bool value);
};

#endif // _BULLET_H_