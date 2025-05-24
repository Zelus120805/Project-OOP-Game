#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <map>
#include "Map.h"
#include "Weapon.h"

class Weapon;

class Player {
protected:
    sf::SoundBuffer buffer;
    sf::Sound sound;

    float dx, dy;
    sf::FloatRect rect;
    bool onGround;
    float currentFrame;

    bool isHit = false;
    int flashCount = 0;
    sf::Clock hitClock;

    sf::Sprite _playerSet;

    Weapon* _weapon = nullptr;

    // Direction
    bool _checkLeft;
    bool _checkRight;
    bool _checkUp;
    bool _checkDown;

    sf::Sprite _bulletSet;

    float _hpPlayer;
    float _hp;
    bool _isDamagedTaken;

    // Quản lí Weapon
    std::map<std::string, std::unique_ptr<Weapon>> _skills;
    std::vector<std::unique_ptr<Weapon>> _weapons;
protected:
    void initSound();
    void isAttacked();
public:
    virtual ~Player();
    virtual void setPlayer(float x, float y) = 0;
    virtual void update(float time, const std::vector<std::string>& tileMap, sf::RenderWindow& window) = 0;
    virtual void setSpriteByPose(const std::string& pose, float currentFrame) = 0;
public:
    sf::FloatRect getRect() const;
    sf::Sprite& getPlayerSprite();
    sf::Sprite& getBulletSprite();
    float getDY() const;
    void setDY(float value);
    void setIsHit(bool value);
    sf::Clock getHitClock();
    void setFlashCount(int value);
    bool getIsHit() const;
    float getHP() const;
    float getHPPlayer() const;

    bool checkLeft() { return _checkLeft; }
    bool checkRight() { return _checkRight; }
    bool checkUp() { return _checkUp; }
    bool checkDown() { return _checkDown; }

    Weapon& getBullet() { return *_weapon; }
    std::vector<std::unique_ptr<Weapon>>& getBullets() { return _weapons; }
public:
    void controlPlayer(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key jump, sf::Keyboard::Key fire);
    void Collision(bool checkVertical, const std::vector<std::string>& tileMap);
    void addBullet(std::unique_ptr<Weapon> bullet);
    void attack(Weapon* weapon);
    void updateWeapons(float time, const std::vector<std::string>& tileMap);
};

class Contra : public Player {
private:
    sf::Texture _text;
    sf::Texture _textBullet;
public:
    Contra();
    ~Contra() override;
public:
    void setPlayer(float x, float y) override;
    void update(float time, const std::vector<std::string>& tileMap, sf::RenderWindow& window) override;
    void setSpriteByPose(const std::string& pose, float currentFrame) override;
public:
    void testisAttacked() {
        if (isHit) {
            isAttacked();
        }
    }
};

class Lugci : public Player {
private:
    sf::Texture _text;
    sf::Texture _textBullet;
public:
    Lugci();
    ~Lugci() override;
public:
    void setPlayer(float x, float y) override;
    void update(float time, const std::vector<std::string>& tileMap, sf::RenderWindow& window) override;
    void setSpriteByPose(const std::string& pose, float currentFrame) override;
public:
    void testisAttacked() {
        if (isHit) {
            isAttacked();
        }
    }
};

#endif // _PLAYER_H_