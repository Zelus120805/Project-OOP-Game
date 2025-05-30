#include "Weapon.h"
#include "Map.h" // để dùng offsetX, offsetY
#include "Player.h"

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
    _damage = GetDamageSpeed(TypeWeapon::Gun);
    _shootCooldown.restart();
}

void Gun::Shoot(float x, float y, WeaponDirection dir) {
    _rect = sf::FloatRect(x, y, 4, 4);
    _direction = dir;
    _active = true;

    switch (_direction) {
        case WeaponDirection::Right:
            _speed = GetSpeedWeapon(TypeWeapon::Gun);
            break;
        case WeaponDirection::Left:
            _speed = -GetSpeedWeapon(TypeWeapon::Gun);
            break;
        case WeaponDirection::Up:
            _speed = 0.f; // Xoay theo Y, không dùng _speed này
            break;
    }
}

void Gun::update(float time, const std::vector<std::string>& currentMap) {
    if (_direction == WeaponDirection::Up) {
        _rect.top -= 0.2f * time; // bay lên
    } else {
        _rect.left += _speed * time; // bay ngang
    }

    _tile.setPosition(_rect.left - offsetX + 1, _rect.top - offsetY - 1);
    collision(currentMap);

    // loại bỏ nếu ra khỏi màn hình
    if (_rect.left < 0 || _rect.left > offsetX + 800.f ||
        _rect.top < 0 || _rect.top > offsetY + 800.f) {
        _active = false;
    }
}

void Gun::attack(Player& player) {
    if (_shootCooldown.getElapsedTime().asMilliseconds() >= GetCoolDownWeapon(TypeWeapon::Gun)) {
        auto bullet = std::make_unique<Gun>();

        if (player.getPose() == PlayerPose::Up) {
            bullet->Shoot(player.getRect().left + player.getRect().width / 2 - 3, player.getRect().top, WeaponDirection::Up);
        } else if (player.getDirection() == Direction::Right) {
            if (player.getPose() != PlayerPose::Down) {
                bullet->Shoot(player.getRect().left + player.getRect().width / 2 + 8, player.getRect().top + player.getRect().height / 2 - 10, WeaponDirection::Right);
            } else {
                bullet->Shoot(player.getRect().left + player.getRect().width / 2 + 8, player.getRect().top + player.getRect().height / 2 - 6, WeaponDirection::Right);
            }
        } else {
            if (player.getPose() != PlayerPose::Down) {
                bullet->Shoot(player.getRect().left - 1, player.getRect().top + player.getRect().height / 2 - 10, WeaponDirection::Left);
            } else {
                bullet->Shoot(player.getRect().left - 1, player.getRect().top + player.getRect().height / 2 - 6, WeaponDirection::Left);
            }
        }

        player.addBullet(std::move(bullet));  // giả sử Contra có addBullet()
        _shootCooldown.restart();
    }
}