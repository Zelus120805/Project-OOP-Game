#include "Enemy.h"

using namespace sf;

Enemy::Enemy() {
    dx = 0.05f;
    dy = 0;
    currentFrame = 0;
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

    // for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++) {
    //     for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
    //         if (tileMap[i][j] == 'P' || tileMap[i][j] == '0') {
    //             if (dx > 0) { 
    //                 rect.left = j * 16 - rect.width; dx = -dx; 
    //             }
    //             else if (dx < 0) { 
    //                 rect.left = j * 16 + 16; dx = -dx; 
    //             }
    //         }
    //     }
    // }
}

// Getters
FloatRect Enemy::getRect() const { return rect; }

Sprite Enemy::getSprite() const { return _enemySprite; }

bool Enemy::isAlive() const { return life; }

float Enemy::getDX() const { return dx; }

// Setters
void Enemy::setDX(float value) { dx = value; }

void Enemy::setAlive(bool value) { life = value; }

void Enemy::takeDamage(float damage) {
    _hp -= damage;
    if (_hp <= 0) {
        dx = 0;
        life = false;
        dx = 0;
    }
}

void SlimeEnemy::setEnemy(int x, int y) {
    if (!_enemyTexture.loadFromFile("Enemy/slime.png")) {
        std::cerr << "Error loading slime.png\n";
    }

    _enemySprite.setTexture(_enemyTexture);
    _enemySprite.scale(1.5f, 1.5f);
    _enemySprite.setOrigin(0.f, 0.f);   // default không lật
    rect = sf::FloatRect(x, y, 18, 18);
    _enemySpeed = 0.02f;
    dx = dy = _enemySpeed;
    _isMovingRight = true;
    // _enemySprite.setTextureRect(sf::IntRect(0, 0, 16, 16)); // frame đầu tiên
}

void SlimeEnemy::updateEnemy(float time, const std::vector<std::string>& tileMap) {
    if (!life) return;

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
    currentFrame += 0.005f * time;
    if (currentFrame >= maxFrame) currentFrame -= maxFrame;

    int frameIndex = static_cast<int>(currentFrame);
    int frameY = 1; // nếu có nhiều hàng animation thì thay đổi cái này

    if (_isMovingRight) {
        _enemySprite.setScale(1.5f, 1.5f);
        _enemySprite.setOrigin(0.f, 0.f);
    } else {
        _enemySprite.setScale(-1.5f, 1.5f);
        _enemySprite.setOrigin(FRAME_WIDTH, 0.f); // để giữ vị trí khi lật trái
    }

    _enemySprite.setTextureRect(sf::IntRect(frameIndex * FRAME_WIDTH, frameY * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT));

    // Cập nhật vị trí sprite
    _enemySprite.setPosition(rect.left - offsetX, rect.top - offsetY);
}