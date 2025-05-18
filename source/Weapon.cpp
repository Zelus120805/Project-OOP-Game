#include "Weapon.h"
#include "Map.h" // để dùng offsetX, offsetY

void Weapon::update(float time, const std::vector<std::string>& currentMap) {
    _rect.left += _speed * time;
    _tile.setPosition(_rect.left - offsetX, _rect.top - offsetY);
    collision(currentMap);

    if (_tile.getPosition().x < 0 || _tile.getPosition().x > offsetX + 400.f) {
        _active = false;
    }
}

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

 void Gun::Shoot(float x, float y, bool goingRight) {
    _rect = sf::FloatRect(x, y, 6, 6);
    _speed = goingRight ? 0.15f : -0.15f;
    _active = true;
}