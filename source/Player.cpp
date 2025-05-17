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
        if (!_isDamagedTaken) {
            _isDamagedTaken = true;
            if (_hp > 0.f)
                _hp -= 10.f;
            else
                _hp = 0.f;
            std::cout << "Player HP: " << _hp << std::endl;
        }

        if (hitClock.getElapsedTime().asMilliseconds() > 200) {
            if (flashCount % 2 == 0)
                _playerSet.setColor(sf::Color::Red);
            else
                _playerSet.setColor(sf::Color::White);

            flashCount++;
            hitClock.restart();

            if (flashCount >= 6) {
                _isDamagedTaken = false;
                isHit = false;
                flashCount = 0;
                _playerSet.setColor(sf::Color::White);
            }
        }
    }
}

// --- Constructor / Destructor ---
Player::~Player() {
    if (_weapon) {
        delete _weapon;
        _weapon = nullptr;
    }
}

// --- Getters / Setters ---
sf::FloatRect Player::getRect() const { return rect; }
sf::Sprite& Player::getPlayerSprite() { return _playerSet; }
sf::Sprite& Player::getBulletSprite() { return _bulletSet; }
float Player::getDY() const { return dy; }
void Player::setDY(float value) { dy = value; }
void Player::setIsHit(bool value) { isHit = value; }
sf::Clock Player::getHitClock() { return hitClock; }
void Player::setFlashCount(int value) { flashCount = value; }
bool Player::getIsHit() const { return isHit; }
float Player::getHP() const { return _hp; }
float Player::getHPPlayer() const { return _hpPlayer; }

// --- Control / Update ---
void Player::controlPlayer(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key jump, sf::Keyboard::Key fire) {
    if (sf::Keyboard::isKeyPressed(left)) {
        dx = -0.05;
        _checkLeft = true;
        _checkRight = false;
        _checkUp = false;
        _checkDown = false;
    }
    if (sf::Keyboard::isKeyPressed(right)) {
        dx = 0.05;
        _checkLeft = false;
        _checkRight = true;
        _checkUp = false;
        _checkDown = false;
    }
    if (sf::Keyboard::isKeyPressed(up)) {
        _checkUp = true;
        _checkDown = false;
    }
    if (sf::Keyboard::isKeyPressed(down)) {
        _checkUp = false;
        _checkDown = true;
    }
    if (sf::Keyboard::isKeyPressed(jump)) {
        if (onGround) {
            dy = -0.3;
            onGround = false;
            sound.play();
        }

        _checkUp = false;
        _checkDown = false;
    }
    if (sf::Keyboard::isKeyPressed(fire)) {
        attack();
    }
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


// Player Mario
Contra::Contra() {
    _weapon = new Gun();
    _hp = _hpPlayer = 100.f;
    _isDamagedTaken = false;
    onGround = false;
    initSound();
}

Contra::~Contra() { }

Weapon& Contra::getBullet() { return *_weapon; }

std::vector<Gun>& Contra::getBullets() { return _bullets; }

void Contra::setPlayer(float x, float y) {
    if (!_text.loadFromFile("Player/Contra.png")) {
        std::cerr << "Error loading Contra.png\n";
    }

    _playerSet.setTexture(_text);
    _bulletSet.setTexture(_text);

    rect = sf::FloatRect(x, y, 20, 35);
    dx = dy = 0;
    currentFrame = 0;
    _checkLeft = false;
    _checkRight = true;
    _checkUp = false;
    _checkDown = false;

    _playerSet.setTextureRect(sf::IntRect(24 * 8, 8 - 5, 30, 32 + 4));
    _bulletSet.setTextureRect(sf::IntRect(51 * 8 - 2, 8 * 2 + 4, 6, 6));
}

void Contra::update(float time, const std::vector<std::string>& tileMap, sf::RenderWindow& window) {
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
        setSpriteByPose("run_right", currentFrame);
    } else if (dx < 0) {
        setSpriteByPose("run_left", currentFrame);
    } else {
        if (!_checkDown && !_checkUp) {
            if (_checkLeft)
                setSpriteByPose("left", currentFrame);
            else
                setSpriteByPose("right", currentFrame);
        } else if (_checkUp) {
            setSpriteByPose("up", currentFrame);
        }  else if (_checkDown) {
            // Tính kích thước nằm
            int targetWidth = 34; // width khi nằm
            int diff = targetWidth - 24; // chênh lệch so với khi đứng

            int i1 = rect.top / 16;
            int i2 = (rect.top + rect.height - 1) / 16;
            bool touchingWall = false;

            if (_checkRight) {
                int j = (rect.left + targetWidth - 1) / 16;
                for (int i = i1; i <= i2; i++) {
                    if (tileMap[i][j] != ' ') {
                        touchingWall = true;
                        break;
                    }
                }

                if (touchingWall) {
                    rect.left -= diff; // đẩy ra xa đủ để nằm không bị cấn
                }
            } else { // đang quay trái
                int j = (rect.left - diff) / 16;
                for (int i = i1; i <= i2; i++) {
                    if (tileMap[i][j] != ' ') {
                        touchingWall = true;
                        break;
                    }
                }

                if (touchingWall) {
                    rect.left += diff; // đẩy ra phải
                }
            }

            setSpriteByPose("down", currentFrame); // cuối cùng mới pose sau khi đẩy
        }
    }

    dx = 0;

    isAttacked();
}

void Contra::setSpriteByPose(const std::string& pose, float currentFrame) {
    // Kích thước sprite mặc định
    int spriteX = 0, spriteY = 0, width = 24, height = 32;

    if (pose == "run_right") {
        spriteX = 20 * 8 - 40 * int(currentFrame);
        spriteY = 8 - 4;
        width = 24;
        height = 32 + 4;
    } else if (pose == "run_left") {
        spriteX = 20 * 8 - 40 * int(currentFrame) + 24;
        spriteY = 8 - 4;
        width = -24;
        height = 32 + 4;
    } else if (pose == "right") {
        spriteX = 25 * 8 - 2;
        spriteY = 8 - 4;
        width = 24;
        height = 32 + 4;
    } else if (pose == "left") {
        spriteX = 25 * 8 + 24 - 2;
        spriteY = 8 - 4;
        width = -24;
        height = 32 + 4;
    } else if (pose == "up") {
        if (_checkRight) {
            spriteX = 40 * 8 + 4;
            spriteY = 0;
            width = 18;
            height = 46;
        }
        else {
            spriteX = 40 * 8 + 4 + 18;
            spriteY = 0;
            width = -18;
            height = 46;
        }
    } else if (pose == "down") {
        if (_checkRight) {
            spriteX = 34 * 8;
            spriteY = 2 * 8 - 1;
            width = 34;
            height = 17;
        }
        else {
            spriteX = 34 * 8 + 34;
            spriteY = 2 * 8 - 1;
            width = -34;
            height = 17;
        }
    }

    // Set sprite texture rect
    _playerSet.setTextureRect(sf::IntRect(spriteX, spriteY, width, height));

    // Set sprite position (chỉnh thấp nếu cần)
    _playerSet.setPosition(rect.left - offsetX, rect.top - offsetY);

    float bottom = rect.top + rect.height;  // Giữ vị trí đáy
    if (pose == "right" || pose == "left" || pose == "run_right" || pose == "run_left") {
        rect.width = 20;
        rect.height = 35;
    } else {
        rect.width = std::abs(width);
        rect.height = std::abs(height);
    }

    rect.top = bottom - rect.height; // Giữ đáy không đổi
}

void Contra::updateBullets(float time, const std::vector<std::string>& tileMap) {
    for (auto& b : _bullets)
        b.update(time, tileMap);

    _bullets.erase(
        std::remove_if(_bullets.begin(), _bullets.end(),
                       [](const Gun& b) { return !b.isActive(); }),
        _bullets.end());
}

void Contra::attack() {
    if (_shootCooldown.getElapsedTime().asMilliseconds() >= 500) {
        Gun bullet;
        if (_checkRight) {
            bullet.Shoot(rect.left + rect.width / 2 + 8, rect.top + rect.height / 2 - 6, _checkRight);
        } else {
            bullet.Shoot(rect.left - 1, rect.top + rect.height / 2 - 6, _checkRight);
        }
        _bullets.push_back(bullet);
        _shootCooldown.restart();
    }
}



