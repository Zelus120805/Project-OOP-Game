#include "Player.h"
#include <iostream>
#include "Map.h"

// --- Hàm private ---
void Player::initSound() {
    if (!buffer.loadFromFile("Sound/Jump.ogg")) {
        std::cerr << "Missing Jump.ogg\n";
    }
    sound.setBuffer(buffer);
}

void Player::isAttacked() {
    if (isHit) {
        if (hitClock.getElapsedTime().asMilliseconds() > 200) {
            if (flashCount % 2 == 0)
                _playerSet.setColor(sf::Color::Red);
            else
                _playerSet.setColor(sf::Color::White);

            flashCount++;
            hitClock.restart();

            if (flashCount >= 6) {
                isHit = false;
                flashCount = 0;
                _playerSet.setColor(sf::Color::White);
            }
        }
    }
}

// --- Constructor / Destructor ---
Player::Player() {
    _hp = 100;
    onGround = false;
    initSound();
}

Player::~Player() { }

// --- Getters / Setters ---
sf::FloatRect Player::getRect() const { return rect; }
sf::Sprite& Player::getPlayerSprite() { return _playerSet; }
Bullet& Player::getBullet() { return _bullet; }
sf::Sprite& Player::getBulletSprite() { return _bulletSet; }
float Player::getDY() const { return dy; }
void Player::setDY(float value) { dy = value; }
void Player::setIsHit(bool value) { isHit = value; }
sf::Clock Player::getHitClock() { return hitClock; }
void Player::setFlashCount(int value) { flashCount = value; }
bool Player::getIsHit() const { return isHit; }
std::vector<Bullet>& Player::getBullets() { return _bullets; }

// --- Setup ---
void Player::setPlayer(float x, float y) {
    if (!_text.loadFromFile("Player/Contra.png")) {
        std::cerr << "Error loading Contra.png\n";
    }

    _playerSet.setTexture(_text);
    _bulletSet.setTexture(_text);

    rect = sf::FloatRect(x, y, 25, 35);
    dx = dy = 0;
    currentFrame = 0;
    _checkRight = true;

    _playerSet.setTextureRect(sf::IntRect(24 * 8, 8 - 5, 30, 32 + 4));
    _bulletSet.setTextureRect(sf::IntRect(51 * 8 - 2, 8 * 2 + 4, 6, 6));
}

// --- Bullet ---
void Player::shoot() {
    if (_shootCooldown.getElapsedTime().asMilliseconds() >= 500) {
        Bullet bullet;
        if (_checkRight) {
            bullet.Shoot(rect.left + rect.width / 2 + 12, rect.top + rect.height / 2 - 6, _checkRight);
        } else {
            bullet.Shoot(rect.left - 2, rect.top + rect.height / 2 - 6, _checkRight);
        }
        _bullets.push_back(bullet);
        _shootCooldown.restart();
    }
}

void Player::updateBullets(float time, const std::vector<std::string>& tileMap) {
    for (auto& b : _bullets)
        b.update(time, tileMap);

    _bullets.erase(
        std::remove_if(_bullets.begin(), _bullets.end(),
                       [](const Bullet& b) { return !b.isActive(); }),
        _bullets.end());
}

// --- Control / Update ---
void Player::controlPlayer(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key up, sf::Keyboard::Key fire) {
    if (sf::Keyboard::isKeyPressed(left)) {
        dx = -0.05;
        _checkRight = false;
    }
    if (sf::Keyboard::isKeyPressed(right)) {
        dx = 0.05;
        _checkRight = true;
    }
    if (sf::Keyboard::isKeyPressed(up)) {
        if (onGround) {
            dy = -0.3;
            onGround = false;
            sound.play();
        }
    }
    if (sf::Keyboard::isKeyPressed(fire)) {
        shoot();
    }
}

void Player::update(float time, const std::vector<std::string>& tileMap, sf::RenderWindow& window) {
    rect.left += dx * time;
    Collision(false, tileMap);

    if (!onGround)
        dy += 0.0005f * time;
    rect.top += dy * time;
    onGround = false;
    Collision(true, tileMap);

    currentFrame += time * 0.005f;
    if (currentFrame > 5)
        currentFrame = 0;

    if (dx > 0) {
        _playerSet.setTextureRect(sf::IntRect(19 * 8 - 40 * int(currentFrame), 8 - 5, 30, 32 + 4));
    } else if (dx < 0) {
        _playerSet.setTextureRect(sf::IntRect(19 * 8 - 40 * int(currentFrame) + 30, 8 - 5, -30, 32 + 4));
    } else {
        if (_checkRight)
            _playerSet.setTextureRect(sf::IntRect(24 * 8, 8 - 5, 30, 32 + 4));
        else
            _playerSet.setTextureRect(sf::IntRect(24 * 8 + 30, 8 - 5, -30, 32 + 4));
    }

    _playerSet.setPosition(rect.left - offsetX, rect.top - offsetY);

    dx = 0;

    isAttacked();
}

// --- Collision ---
void Player::Collision(bool checkVertical, const std::vector<std::string>& tileMap) {
    for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++) {
        for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
            if (tileMap[i][j] == 'P' || tileMap[i][j] == 'k' ||
                tileMap[i][j] == '0' || tileMap[i][j] == 'r' ||
                tileMap[i][j] == 't') {

                if (dy > 0 && checkVertical) {
                    rect.top = i * 16 - rect.height;
                    dy = 0;
                    onGround = true;
                }

                if (dy < 0 && checkVertical) {
                    rect.top = i * 16 + 16;
                    dy = 0;
                }

                if (dx > 0 && !checkVertical)
                    rect.left = j * 16 - rect.width;

                if (dx < 0 && !checkVertical)
                    rect.left = j * 16 + 16;
            }
        }
    }
}
