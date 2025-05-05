#include "Enemy.h"

using namespace sf;

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::setEnemy(Texture& image, int x, int y) {
    sprite.setTexture(image);
    rect = FloatRect(x, y, 16, 16);
    dx = 0.05f;
    currentFrame = 0;
    life = true;
}

void Enemy::update(float time, const std::vector<std::string>& tileMap) {
    rect.left += dx * time;
    Collision(tileMap);

    currentFrame += time * 0.005f;
    if (currentFrame > 2) currentFrame -= 2;

    if (life)
        sprite.setTextureRect(IntRect(18 * int(currentFrame), 0, 16, 16));
    else
        sprite.setTextureRect(IntRect(58, 0, 16, 16));

    sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
}

void Enemy::Collision(const std::vector<std::string>& tileMap) {
    for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++) {
        for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
            if (tileMap[i][j] == 'P' || tileMap[i][j] == '0') {
                if (dx > 0) {
                    rect.left = j * 16 - rect.width;
                    dx = -dx;
                } else if (dx < 0) {
                    rect.left = j * 16 + 16;
                    dx = -dx;
                }
            }
        }
    }
}

// Getters
FloatRect Enemy::getRect() const { return rect; }

Sprite Enemy::getSprite() const { return sprite; }

bool Enemy::isAlive() const { return life; }

float Enemy::getDX() const { return dx; }

// Setters
void Enemy::setDX(float value) { dx = value; }

void Enemy::setAlive(bool value) { life = value; }
