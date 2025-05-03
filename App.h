#ifndef _APP_H_
#define _APP_H_
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
using namespace sf;

class App {
private:
    RenderWindow _window;
    Texture _tileSet;

    Map _map;
    Player _player; // dùng constructor rỗng tạm
    Enemy _enemy;

    Music _music;
    Clock _clock;
public:
    App() : _window(VideoMode(450, 300), "Game"), _player() {
        if (!_tileSet.loadFromFile("Player/Mario_tileset.png")) {
            std::cerr << "Error loading Mario_Tileset.png\n";
        }

        _player.setPlayer(120, 120);

        auto& currentMap = _map.getMap(0);
        for (int i = 0; i < currentMap.size(); ++i) {
            for (int j = 0; j < currentMap[i].size(); ++j) {
                if (currentMap[i][j] == 't') { // Tại kí tự 't' là enemy
                    _enemy.setEnemy(_tileSet, j * 16, i * 16); // Đặt enemy tại vị trí tương ứng
                }
            }
        }

        if (!_music.openFromFile("Sound/Mario_Theme.ogg"))
            std::cerr << "Missing Mario_Theme.ogg\n";
        _music.setLoop(true);
        _music.play();
    }
    
    virtual ~App() { }
    
    void run() {
        auto& currentMap = _map.getMap(0);

        while (_window.isOpen()) {
            float time = _clock.restart().asMicroseconds() / 500.f;
            if (time > 20) time = 20;

            Event event;
            while (_window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    _window.close();
            }

            if (_player.getRect().left > 200)
                offsetX = _player.getRect().left - 200;

            _window.clear(Color(107, 140, 255));
            //_window.clear(Color::Blue);

            // Vẽ map
            _map.render(_window, _tileSet);

            // Va chạm Player với enemy
            if (_player.getRect().intersects(_enemy.getRect())) {
                if (_enemy.isAlive()) {
                    if (_player.getDY() > 0) {
                        _enemy.setDX(0);
                        _player.setDY(-0.2f);
                        _enemy.setAlive(false);
                    } 
                    else {
                        if (!_player.getIsHit()) { // chỉ set nếu chưa bị hit
                            _player.setIsHit(true);
                            _player.getHitClock().restart();
                            _player.setFlashCount(0);
                        }
                    }
                }
            }

            _player.controlPlayer(Keyboard::A, Keyboard::D, Keyboard::K, Keyboard::J);

            _player.update(time, currentMap, _window);
            _player.updateBullets(time, currentMap); // cập nhật tất cả đạn
            //_player.getBullet().update(time); // cập nhật đạn
            _enemy.update(time, currentMap);

            _window.draw(_player.getPlayerSprite());
            for (const Bullet& b : _player.getBullets()) {
                if (b.isActive()) {
                    _player.getBulletSprite().setPosition(b.getPosition().x, b.getPosition().y);
                    _window.draw(_player.getBulletSprite());
                }
            }
            _window.draw(_enemy.getSprite());
            _window.display();
        }
    }
};
    

#endif