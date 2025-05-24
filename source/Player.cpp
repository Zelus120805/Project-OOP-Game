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
void Player::controlPlayer(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key jump, sf::Keyboard::Key shoot) {
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
    if (sf::Keyboard::isKeyPressed(shoot)) {
        //std::cout << "Shooting...\n";

        attack(_skills["Gun"].get());
    }
}

// --- Collision ---
void Player::Collision(bool checkVertical, const std::vector<std::string>& tileMap) {
    for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++) {
        if (i < 0 || i >= (int)tileMap.size()) continue; // Check chỉ số i hợp lệ
        for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
            if (j < 0 || j >= (int)tileMap[i].size()) continue; // Check chỉ số j hợp lệ
            if (tileMap[i][j] >= '0' && tileMap[i][j] <= '9') {

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

void Player::addBullet(std::unique_ptr<Weapon> bullet) {
    _weapons.push_back(std::move(bullet));
}

// Atack with weapon
void Player::updateWeapons(float time, const std::vector<std::string>& tileMap) {
    for (auto& weapon : _weapons)
        weapon->update(time, tileMap);

    _weapons.erase(
        std::remove_if(_weapons.begin(), _weapons.end(),
                       [](const std::unique_ptr<Weapon>& b) { return !b->isActive(); }),
        _weapons.end());
}

void Player::attack(Weapon* weapon) {
    if (weapon) {
        weapon->attack(*this);
    }
}

// Player Contra
Contra::Contra() {
    _weapon = new Gun();
    _hp = _hpPlayer = 100.f;
    _isDamagedTaken = false;
    onGround = false;
    initSound();
    _skills["Gun"] = std::make_unique<Gun>();
}

Contra::~Contra() { }

void Contra::setPlayer(float x, float y) {
    if (!_text.loadFromFile("Player/Contra.png")) {
        std::cerr << "Error loading Contra.png\n";
    }

    _playerSet.setTexture(_text);
    _bulletSet.setTexture(_text);

    rect = sf::FloatRect(x, y, 24, 35);
    dx = dy = 0;
    currentFrame = 0;
    _checkLeft = false;
    _checkRight = true;
    _checkUp = false;
    _checkDown = false;

    _playerSet.setTextureRect(sf::IntRect(25 * 8 - 2, 8 - 4, 24, 36));
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
                int j = (rect.left + rect.width + diff) / 16;
                for (int i = i1; i <= i2; i++) {
                    if (tileMap[i][j] >= '0' && tileMap[i][j] <= '9') {
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
                    if (tileMap[i][j] >= '0' && tileMap[i][j] <= '9') {
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

    float bottom = rect.top + rect.height;  // Giữ vị trí đáy

    rect.width = std::abs(width) - 8;
    rect.height = std::abs(height) - 6;

    _playerSet.setPosition(rect.left - offsetX - 8 / 2.f, rect.top - offsetY - 6);

    rect.top = bottom - rect.height; // Giữ đáy không đổi
}

// Player Lugci
Lugci::Lugci() {
    _weapon = new Gun();
    _hp = _hpPlayer = 100.f;
    _isDamagedTaken = false;
    onGround = false;
    initSound();
    _skills["Gun"] = std::make_unique<Gun>();
}

Lugci::~Lugci() { }

void Lugci::setPlayer(float x, float y) {
    if (!_text.loadFromFile("Player/Lugci.png")) {
        std::cerr << "Error loading Lugci.png\n";
    }

    if (!_textBullet.loadFromFile("Player/Contra.png")) {
        std::cerr << "Error loading Contra.png\n";
    }

    _playerSet.setTexture(_text);
    _bulletSet.setTexture(_textBullet);

    rect = sf::FloatRect(x, y, 24, 35);
    dx = dy = 0;
    currentFrame = 0;
    _checkLeft = false;
    _checkRight = true;
    _checkUp = false;
    _checkDown = false;

    _playerSet.setTextureRect(sf::IntRect(18 * 8 + 1, 20, 24, 36));
    _bulletSet.setTextureRect(sf::IntRect(51 * 8 - 2, 8 * 2 + 4, 6, 6));
}

void Lugci::update(float time, const std::vector<std::string>& tileMap, sf::RenderWindow& window) {
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
                int j = (rect.left + rect.width + diff) / 16;
                for (int i = i1; i <= i2; i++) {
                    if (tileMap[i][j] >= '0' && tileMap[i][j] <= '9') {
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
                    if (tileMap[i][j] >= '0' && tileMap[i][j] <= '9') {
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

void Lugci::setSpriteByPose(const std::string& pose, float currentFrame) {
    // Kích thước sprite mặc định
    int spriteX = 0, spriteY = 0, width = 24, height = 32;

    if (pose == "run_right") {
        spriteX = 18 * 8 - 1 + 24 * int(currentFrame);
        spriteY = 17 * 8 - 4;
        width = 24;
        height = 32 + 4;
    } else if (pose == "run_left") {
        spriteX = 18 * 8 - 1 + 24 * int(currentFrame) + 24;
        spriteY = 17 * 8 - 4;
        width = -24;
        height = 32 + 4;
    } else if (pose == "right") {
        spriteX = 18 * 8 + 1;
        spriteY = 17;
        width = 24;
        height = 32 + 4;
    } else if (pose == "left") {
        spriteX = 18 * 8 + 1 + 24;
        spriteY = 17;
        width = -24;
        height = 32 + 4;
    } else if (pose == "up") {
        if (_checkRight) {
            spriteX = 25 * 8 + 3;
            spriteY = 7;
            width = 18;
            height = 46;
        }
        else {
            spriteX = 25 * 8 + 3 + 18;
            spriteY = 7;
            width = -18;
            height = 46;
        }
    } else if (pose == "down") {
        if (_checkRight) {
            spriteX = 18 * 8;
            spriteY = 22 * 8 - 1;
            width = 34;
            height = 17;
        }
        else {
            spriteX = 18 * 8 + 34;
            spriteY = 22 * 8 - 1;
            width = -34;
            height = 17;
        }
    }

    // Set sprite texture rect
    _playerSet.setTextureRect(sf::IntRect(spriteX, spriteY, width, height));

    float bottom = rect.top + rect.height;  // Giữ vị trí đáy

    rect.width = std::abs(width) - 8;
    rect.height = std::abs(height) - 6;

    _playerSet.setPosition(rect.left - offsetX - 8 / 2.f, rect.top - offsetY - 6);

    rect.top = bottom - rect.height; // Giữ đáy không đổi
}

