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
    RenderWindow window;
    Texture tileSet;
    Sprite tile;

    Map _map;
    Player _player; // dùng constructor rỗng tạm
    Enemy _enemy;

    Music music;
    Clock clock;
public:
    App() : window(VideoMode(400, 250), "SFML Mario Co-op"), _player() {
        if (!tileSet.loadFromFile("Mario_Tileset.png")) {
            std::cerr << "Error loading Mario_Tileset.png\n";
        }

        tile.setTexture(tileSet);

        _player.setPlayer(tileSet, 120, 120);

        _enemy.set(tileSet, 48 * 16, 13 * 16);

        if (!music.openFromFile("Mario_Theme.ogg"))
            std::cerr << "Missing Mario_Theme.ogg\n";
        music.setLoop(true);
        music.play();
    }
    
    ~App() { }
    
    void run() {
        auto& currentMap = _map.getMap(0);

        while (window.isOpen()) {
            float time = clock.restart().asMicroseconds() / 500.f;
            if (time > 20) time = 20;

            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
            }

            _player.controlPlayer();
            _player.update(time, currentMap);
            _enemy.update(time, currentMap);

            if (_player.getRect().left > 200)
                offsetX = _player.getRect().left - 200;

            window.clear(Color(107, 140, 255));

            for (int i = 0; i < _map.getHeight(); i++) {
                for (int j = 0; j < _map.getWidth(); j++) {
                    char tileChar = currentMap[i][j];
                    if (tileChar == ' ' || tileChar == '0') continue;

                    if (tileChar == 'P') tile.setTextureRect(IntRect(143 - 16 * 3, 112, 16, 16));
                    else if (tileChar == 'k') tile.setTextureRect(IntRect(143, 112, 16, 16));
                    else if (tileChar == 'c') tile.setTextureRect(IntRect(143 - 16, 112, 16, 16));
                    else if (tileChar == 't') tile.setTextureRect(IntRect(0, 47, 32, 48));
                    else if (tileChar == 'g') tile.setTextureRect(IntRect(0, 139, 48, 37));
                    else if (tileChar == 'G') tile.setTextureRect(IntRect(145, 222, 77, 33));
                    else if (tileChar == 'd') tile.setTextureRect(IntRect(0, 106, 74, 21));
                    else if (tileChar == 'w') tile.setTextureRect(IntRect(99, 224, 41, 31));
                    else if (tileChar == 'r') tile.setTextureRect(IntRect(143 - 32, 112, 16, 16));

                    tile.setPosition(j * 16 - offsetX, i * 16 - offsetY);
                    window.draw(tile);
                }
            }

            // Va chạm Mario với enemy
            if (_player.getRect().intersects(_enemy.getRect())) {
                if (_enemy.isAlive()) {
                    if (_player.getDY() > 0) {
                        _enemy.setDX(0);
                        _player.setDY(-0.2f);
                        _enemy.setAlive(false);
                    } 
                    else {
                        _player.setIsHit(true);
                        _player.getHitClock().restart();
                        _player.setFlashCount(0);
                    }
                }
            }

            window.draw(_player.getSprite());
            window.draw(_enemy.getSprite());
            window.display();
        }
    }
};
    

#endif