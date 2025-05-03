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

public:
    Bullet();
    virtual ~Bullet();

    void Shoot(float x, float y, bool goingRight);
    void update(float time, const std::vector<std::string>& currentMap);
    void collision(const std::vector<std::string>& tileMap);

    // Getters & utility
    bool isActive() const;
    void deactivate();
    sf::Sprite getSprite() const;
    sf::Vector2f getPosition() const;
};

#endif // _BULLET_H_