#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include "Map.h"
#include "Weapon.h"

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

    sf::Texture _text;
    sf::Sprite _playerSet;

    Weapon* _weapon = nullptr;

    // Direction
    bool _checkLeft;
    bool _checkRight;
    bool _checkUp;
    bool _checkDown;

    sf::Sprite _bulletSet;
    sf::Clock _shootCooldown;

    float _hpPlayer;
    float _hp;
    bool _isDamagedTaken;
protected:
    void initSound();
    void isAttacked();
public:
    virtual ~Player();
    virtual void setPlayer(float x, float y) = 0;
    virtual void update(float time, const std::vector<std::string>& tileMap, sf::RenderWindow& window) = 0;
    virtual void attack(const std::string& pose) = 0;
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
public:
    void controlPlayer(sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key jump, sf::Keyboard::Key fire);
    void Collision(bool checkVertical, const std::vector<std::string>& tileMap);
};

class Contra : public Player {
private:
    std::vector<std::unique_ptr<Weapon>> _weapons;
public:
    std::vector<std::unique_ptr<Weapon>>& getBullets();
    Weapon& getBullet();
public:
    Contra();
    ~Contra() override;
public:
    void setPlayer(float x, float y) override;
    void update(float time, const std::vector<std::string>& tileMap, sf::RenderWindow& window) override;
public:
    void useGun();
    void useKnife();
public:
    void setSpriteByPose(const std::string& pose, float currentFrame);
    void attack(const std::string& pose) override;
    void updateBullets(float time, const std::vector<std::string>& tileMap);
};

#endif // _PLAYER_H_