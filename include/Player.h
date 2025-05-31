#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <vector>
#include <map>
#include "Map.h"
#include "Weapon.h"
#include "Error.h"
#include "InputConfig.h"

enum class PlayerType {
    Contra,
    Lugci,
};

enum class PlayerPose {
    RunLeft,
    RunRight,
    Up,
    Down,
    Stand,
};

enum class Direction {
    Left,
    Right,
};

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
    float _damageEnemy;

    sf::Sprite _playerSet;

    // Direction
    Direction _dir;
    PlayerPose _pose;

    sf::Sprite _bulletSet;

    float _hpPlayer;
    float _hp;
    bool _isDamagedTaken;

    // Die
    bool _isDying;
    float _dieX, _dieY;
    float _deathElapsed;
    float _deathFrame;
    float _deathFrameSpeed;
    std::vector<sf::IntRect> _deathFrames;
    bool _isDeadCompletely;

    // Quản lí Weapon
    Weapon* _weapon = nullptr;
    std::map<std::string, std::unique_ptr<Weapon>> _skills;
    std::vector<std::unique_ptr<Weapon>> _weapons;
protected:
    void initSound();
public:
    virtual ~Player();
    virtual void setPlayer(float x, float y) = 0;
    virtual void update(float time, const std::vector<std::string>& tileMap, sf::RenderWindow& window) = 0;
    virtual void setSpriteByPose(PlayerPose pose, float currentFrame) = 0;
    virtual void controlPlayer(const std::map<PlayerAction, sf::Keyboard::Key>& keyMap) = 0;
public:
    sf::FloatRect getRect() const;
    sf::Sprite& getPlayerSprite();
    sf::Sprite& getBulletSprite();
    float getDY() const;
    void setDY(float value);
    void setIsHit(bool value, const float&);
    sf::Clock getHitClock();
    bool getIsHit() const;
    float getHP() const;
    float getHPPlayer() const;
    bool finishPlayer() const;
    
    Direction getDirection() const;
    PlayerPose getPose() const;

    Weapon& getBullet() { return *_weapon; }
    std::vector<std::unique_ptr<Weapon>>& getBullets() { return _weapons; }
public:
    void takeDamage();
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
    void setSpriteByPose(PlayerPose pose, float currentFrame) override;
    void controlPlayer(const std::map<PlayerAction, sf::Keyboard::Key>& keyMap) override;
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
    void setSpriteByPose(PlayerPose pose, float currentFrame) override;
    void controlPlayer(const std::map<PlayerAction, sf::Keyboard::Key>& keyMap) override;
};

#endif // _PLAYER_H_