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
        if (!_tileSet.loadFromFile("Mario_Tileset.png")) {
            std::cerr << "Error loading Mario_Tileset.png\n";
        }

        _player.setPlayer(120, 120);

        _enemy.setEnemy(_tileSet, 48 * 16, 13 * 16);

        if (!_music.openFromFile("Mario_Theme.ogg"))
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

            _player.controlPlayer();
            _player.update(time, currentMap);
            _enemy.update(time, currentMap);

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

            _window.draw(_player.getSprite());
            _window.draw(_enemy.getSprite());
            _window.display();
        }
    }
};
    

#endif