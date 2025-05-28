#include "Enemy.h"

using namespace sf;

Enemy::Enemy() {
    dx = 0.1f;
    dy = 0;
    life = true;
    _hp = 100;
    _onGround = false;
}

Enemy::~Enemy() { }

// --- Collision ---
void Enemy::Collision(bool checkVertical, const std::vector<std::string>& tileMap) {
    for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++) {
        for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
            if (tileMap[i][j] >= '0' && tileMap[i][j] <= '9') {
                if (checkVertical) {
                    if (dy > 0) {
                        rect.top = i * 16 - rect.height;
                        dy = 0;
                        _onGround = true;
                    }
                    if (dy < 0) {
                        rect.top = i * 16 + 16;
                        dy = 0;
                    }
                }

                if (!checkVertical) {
                    if (dx > 0) {
                        rect.left = j * 16 - rect.width;
                        dx = -dx;
                    }
                    else if (dx < 0) {
                        rect.left = j * 16 + 16;
                        dx = -dx;
                    }
                }
            }
        }
    }
}

// Getters
FloatRect Enemy::getRect() const { return rect; }

Sprite Enemy::getSprite() const { return _enemySprite; }

bool Enemy::isAlive() const { return life; }

float Enemy::getDX() const { return dx; }

float Enemy::getDamage() const { return _damge; }

// Setters
void Enemy::setDX(float value) { dx = value; }

void Enemy::takeDamage(float damage) {
    _hp -= damage;
    if (_hp <= 0) {
        dx = 0;
        life = false;
        dx = 0;
    }
}
SlimeEnemy::SlimeEnemy() {
    _damge = 30.f;
}

void SlimeEnemy::setEnemy(int x, int y) {
    if (!_enemyTexture.loadFromFile("Enemy/slime.png")) {
        std::cerr << "Error loading slime.png\n";
    }

    // Tạo các frame cho animation
    for (int i = 0; i < 4; ++i) {
        _idleFrames.push_back(sf::IntRect(i * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT));
    }
    for (int i = 0; i < 4; ++i) {
        _walkFrames.push_back(sf::IntRect(i * FRAME_WIDTH, FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));
    }
    for (int i = 0; i < 5; ++i) {
        _attackFrames.push_back(sf::IntRect(i * FRAME_WIDTH, FRAME_HEIGHT * 2, FRAME_WIDTH, FRAME_HEIGHT));
    }
    for (int i = 0; i < 6; ++i) {
        _deathFrames.push_back(sf::IntRect(i * FRAME_WIDTH, FRAME_HEIGHT * 3, FRAME_WIDTH, FRAME_HEIGHT));
    }
    
    _enemySprite.setTexture(_enemyTexture);
    _enemySprite.setTextureRect(_idleFrames[0]);  // Bắt đầu với frame idle đầu tiên
    _enemySprite.scale(1.5f, 1.5f);
    _enemySprite.setOrigin(8.f, 0.f);   // default không lật
    rect = sf::FloatRect(x, y, 16, 16);
    _enemySpeed = 0.01f;
    dx = dy = _enemySpeed;
    _isMovingRight = true;
    _isAttacking = false;
    _currentFrame = 0.f;
    _frameSpeed = 2.f;  // FPS animation
    _attackDuration = 0.5f;
    _attackElapsed = 0.f;
}

void SlimeEnemy::updateEnemy(float time, const std::vector<std::string>& tileMap, Player& player) {
    if (_isDying) {
        _deathElapsed += time / 1000.f;
        _deathFrame += _deathFrameSpeed * time / 1000.f;

        if (_deathFrame >= _deathFrames.size())
            _deathFrame = _deathFrames.size() - 1;

        _enemySprite.setTextureRect(_deathFrames[static_cast<int>(_deathFrame)]);
        _enemySprite.setPosition(rect.left - offsetX, rect.top - offsetY);

        return; // Không update thêm gì nữa nếu đang chết
    }

    // Tính khoảng cách giữa Enemy và Player
    sf::Vector2f enemyPos = sf::Vector2f(rect.left + rect.width / 2, rect.top + rect.height / 2);
    sf::Vector2f playerPos = sf::Vector2f(player.getRect().left + player.getRect().width / 2,
                                          player.getRect().top + player.getRect().height / 2);
    // Tính khoảng cách X và Y
    float distanceX = std::abs(enemyPos.x - playerPos.x);
    float distanceY = std::abs(enemyPos.y - playerPos.y);

    // Nếu player trong khoảng phát hiện và slime không tấn công thì bắt đầu tấn công
    if (distanceX < _detectionRange && distanceY < (rect.height + player.getRect().height) * 0.5f) {
        if (!isAttacking()) {
            attack(playerPos.x);
        }
    }

    // Cập nhật thời gian tấn công
    if (isAttacking()) {
        _attackElapsed += time / 10000.f;
        _currentFrame += _frameSpeed * time / 10000.f;
        if (_currentFrame >= _attackFrames.size()) _currentFrame = 0;

        _enemySprite.setTextureRect(_attackFrames[static_cast<int>(_currentFrame)]);

        if (_attackElapsed >= _attackDuration) {
            _isAttacking = false;
            _currentFrame = 0.f;
            dx = _enemySpeed * (_isMovingRight ? 1 : -1); // Reset tốc độ bình thường
        }
    } else {
        _currentFrame += _frameSpeed * time / 1000.f;
        if (_currentFrame >= _idleFrames.size()) _currentFrame = 0;

        _enemySprite.setTextureRect(_idleFrames[static_cast<int>(_currentFrame)]);
    }

    // Cập nhật vị trí theo trục X
    rect.left += dx * time;
    Collision(false, tileMap);

    // Trọng lực
    if (!_onGround)
        dy += 0.0005f * time;
    rect.top += dy * time;
    _onGround = false;
    Collision(true, tileMap);

    // Cập nhật frame animation
    if (dx > 0) {
        _enemySprite.setTextureRect(_walkFrames[static_cast<int>(_currentFrame)]);
        _enemySprite.setScale(1.5f, 1.5f); // Scale sprite
        _isMovingRight = true;
        _isMovingLeft = false;
    } else if (dx < 0) {
        _enemySprite.setTextureRect(_walkFrames[static_cast<int>(_currentFrame)]);
        _enemySprite.setScale(-1.5f, 1.5f); // Lật sprite
        _isMovingRight = false;
        _isMovingLeft = true;
    } else {
        if (_isMovingRight) {
            _enemySprite.setTextureRect(_idleFrames[static_cast<int>(_currentFrame)]);
            _enemySprite.setScale(1.5f, 1.5f); // Scale sprite
        } else if (_isMovingLeft) {
            _enemySprite.setTextureRect(_idleFrames[static_cast<int>(_currentFrame)]);
            _enemySprite.setScale(-1.5f, 1.5f); // Lật sprite
        }
    }

    // Cập nhật vị trí sprite
    _enemySprite.setPosition(rect.left - offsetX, rect.top - offsetY);
}

void SlimeEnemy::attack(float playerX) {
    if (!isAttacking() && life) {
        _isAttacking = true;
        _attackElapsed = 0.f;
        _currentFrame = 0.f;

        // Dash về phía Player
        if (playerX > rect.left) {
            dx = _enemySpeed * 3.7;  // Dash phải
        } else {
            dx = -_enemySpeed * 3.7; // Dash trái
        }

        dy = 0.f;
        // std::cout << "SlimeEnemy attack animation!\n";
    }
}

bool SlimeEnemy::isAttacking() const { return _isAttacking; }

bool SlimeEnemy::isDying() const { return _isDying; }

void SlimeEnemy::takeDamage(float damage) {
    _hp -= damage;
    if (_hp <= 0 && !_isDying) {
        die(); // Kích hoạt animation chết
    }
}

void SlimeEnemy::die() {
    life = false;    // Enemy không thể gây sát thương nữa
    dx = 0;          // Không di chuyển
    dy = 0;
    _isDying = true; // Bắt đầu animation chết
    _currentFrame = 0.f;
    _deathElapsed = 0.f;
}