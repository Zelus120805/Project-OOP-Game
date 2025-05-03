#ifndef _APP_H_
#define _APP_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Map.h"

class App {
private:
    sf::RenderWindow _window;
    sf::Texture _tileSet;

    Map _map;
    Player _player;
    Enemy _enemy;

    sf::Music _music;
    sf::Clock _clock;

    void init();
    void handleEvents();
    void update(float time);
    void render();

public:
    App();
    virtual ~App();

    void run();
};

#endif // _APP_H_