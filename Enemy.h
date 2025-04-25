#ifndef _ENRMY_H_
#define _ENRMY_H_

#include <SFML/Graphics.hpp>
#include "Map.h"
using namespace sf;

class Enemy {
private:
    float dx, dy;
    FloatRect rect;
    Sprite sprite;
    float currentFrame;
    bool life;
public:
    FloatRect getRect() { return rect; }
    
    Sprite getSprite() { return sprite; }

    bool isAlive() { return life; }

    FloatRect getRect() const { return rect; }

    float getDX() const { return dx; }

    void setDX(float value) { dx = value; }

    bool isAlive() const { return life; }

    void setAlive(bool value) { life = value; }
public:
    Enemy() { }
    void set(Texture &image, int x, int y) {
        sprite.setTexture(image);
        rect = FloatRect(x, y, 16, 16);
        dx = 0.05f;
        currentFrame = 0;
        life = true;
    }

    ~Enemy() { }
    
    void update(float time, const std::vector<std::string>& tileMap) {
        rect.left += dx * time;
        Collision(tileMap);
    
        currentFrame += time * 0.005f;
        if (currentFrame > 2) currentFrame -= 2;
    
        sprite.setTextureRect(life ? IntRect(18 * int(currentFrame), 0, 16, 16) : IntRect(58, 0, 16, 16));
        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
    }
    
    void Collision(const std::vector<std::string>& tileMap) {
        for (int i = rect.top / 16; i < (rect.top + rect.height) / 16; i++)
            for (int j = rect.left / 16; j < (rect.left + rect.width) / 16; j++) {
                if (tileMap[i][j] == 'P' || tileMap[i][j] == '0') {
                    if (dx > 0) { 
                        rect.left = j * 16 - rect.width; dx = -dx; 
                    }
                    else if (dx < 0) { 
                        rect.left = j * 16 + 16; dx = -dx; 
                    }
                }
            }
        }
    };

#endif // ENRMY_H
