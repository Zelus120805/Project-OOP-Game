#include "Weapon.h"
#include "Map.h" // để dùng offsetX, offsetY

void Weapon::collision(const std::vector<std::string>& tileMap) {
    for (int i = _rect.top / 16; i < (_rect.top + _rect.height) / 16; ++i) {
        for (int j = _rect.left / 16; j < (_rect.left + _rect.width) / 16; ++j) {
            char tile = tileMap[i][j];
            if (tile >= '0' && tile <= '9') {
                _active = false;
            }
        }
    }
}

bool Weapon::isActive() const { return _active; }

void Weapon::deactivate() { _active = false; }

sf::Sprite Weapon::getSprite() const { return _tile; }

sf::Vector2f Weapon::getPosition() const {
    return sf::Vector2f(_tile.getPosition().x, _tile.getPosition().y);
}

sf::FloatRect Weapon::getRect() const { return _rect; }

float Weapon::getDamage() const { return _damage; }

void Weapon::setActive(bool value) { _active = value; }

// class Gun
Gun::Gun() {
    _active = false;
    _damage = 10.f;
}

void Gun::Shoot(float x, float y, BulletDirection dir) {
    _rect = sf::FloatRect(x, y, 6, 6);
    _direction = dir;
    _active = true;

    switch (_direction) {
        case BulletDirection::Right:
            _speed = 0.15f;
            break;
        case BulletDirection::Left:
            _speed = -0.15f;
            break;
        case BulletDirection::Up:
            _speed = 0.f; // Xoay theo Y, không dùng _speed này
            break;
    }
}

void Gun::update(float time, const std::vector<std::string>& currentMap) {
    if (_direction == BulletDirection::Up) {
        _rect.top -= 0.2f * time; // bay lên
    } else {
        _rect.left += _speed * time; // bay ngang
    }

    _tile.setPosition(_rect.left - offsetX, _rect.top - offsetY);
    collision(currentMap);

    // loại bỏ nếu ra khỏi màn hình
    if (_rect.left < 0 || _rect.left > offsetX + 800.f ||
        _rect.top < 0 || _rect.top > offsetY + 800.f) {
        _active = false;
    }
}