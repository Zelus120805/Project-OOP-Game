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

void Enemy::setEnemy(Texture& image, int x, int y) {
    sprite.setTexture(image);
    sprite.scale(1.5, 1.5);
    rect = FloatRect(x, y, 24, 24);
}

void Enemy::update(float time, const std::vector<std::string>& tileMap) {
    rect.left += dx * time;
    Collision(false, tileMap);

    if (!_onGround)
        dy += 0.0005f * time;
    rect.top += dy * time;
    _onGround = false;
    Collision(true, tileMap);

    currentFrame += time * 0.005f;
    if (currentFrame > 2) currentFrame -= 2;

    if (life)
        sprite.setTextureRect(IntRect(18 * int(currentFrame), 0, 16, 16));
    else
        sprite.setTextureRect(IntRect(58, 0, 16, 16));

    sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
}

// --- Collision ---
void Enemy::Collision(bool checkVertical, const std::vector<std::string>& tileMap) {
    for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++) {
        for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
            if (tileMap[i][j] == 'P' || tileMap[i][j] == 'k' ||
                tileMap[i][j] == '0' || tileMap[i][j] == 'r' ||
                tileMap[i][j] == 't') {
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

Sprite Enemy::getSprite() const { return sprite; }

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