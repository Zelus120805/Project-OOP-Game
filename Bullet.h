#ifndef _BULLET_H_
#define _BULLET_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Map.h"
using namespace sf;

class Bullet {
private:
    Texture _texture;
    Sprite _tile;
    FloatRect _rect; // vị trí viên đạn
    float _speed; // tốc độ viên đạn
    bool _active;
public:
    bool isActive() const { return _active; }

    void deactivate() { _active = false; }

    Sprite getSprite() const { return _tile; }
public:
    Bullet() : _active(false) { }

    virtual ~Bullet() { }

    void Shoot(float x, float y, bool goingRight) {
        _rect = FloatRect(x, y, 6, 6); // Set vị trí viên đạn
        // std::cout << "Bullet position: " << x << ", " << y << std::endl;
        _speed = goingRight ? 0.15f : -0.15f;
        _active = true; // thêm dòng này để đạn bắt đầu bay
    }

    void collision(const std::vector<std::string>& tileMap) {
        for (int i = _rect.top / 16; i < (_rect.top + _rect.height) / 16; i++)
            for (int j = _rect.left / 16; j < (_rect.left + _rect.width) / 16; j++) {
                if (tileMap[i][j] == 'P' || tileMap[i][j] == 'k' || tileMap[i][j] == '0' || tileMap[i][j] == 'r' || tileMap[i][j] == 't') {
                    _active = false;
                }
            }
    }

    void update(float time, const std::vector<std::string>& currentMap) {
        _rect.left += _speed * time;
        _tile.setPosition(_rect.left - offsetX, _rect.top - offsetY); // Set vị trí viên đạn
        collision(currentMap); // Kiểm tra va chạm với tileMap

        // Nếu viên đạn bay ra khỏi màn hình, tắt nó đi
        if (_tile.getPosition().x < 0 || _tile.getPosition().x > offsetX + 400.f) {
            _active = false;
        }
    }

    sf::Vector2f getPosition() const {
        return sf::Vector2f(_tile.getPosition().x, _tile.getPosition().y); // hoặc dùng sprite nếu bạn lưu vị trí qua sprite
    }
};

#endif // _BULLET_H_
