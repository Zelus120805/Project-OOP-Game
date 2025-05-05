#include "App.h"

App::App() : _window(sf::VideoMode(450, 300), "Game"), _player() {
    init();
}

App::~App() {}

void App::init() {
    if (!_tileSet.loadFromFile("Player/Mario_tileset.png"))
        std::cerr << "Error loading Mario_Tileset.png\n";

    _player.setPlayer(120, 120);

    auto& currentMap = _map.getMap(0);
    for (int i = 0; i < currentMap.size(); ++i)
        for (int j = 0; j < currentMap[i].size(); ++j)
            if (currentMap[i][j] == 't')
                _enemy.setEnemy(_tileSet, j * 16, i * 16);

    if (!_music.openFromFile("Sound/Mario_Theme.ogg"))
        std::cerr << "Missing Mario_Theme.ogg\n";

    _music.setLoop(true);
    _music.play();
}

void App::handleEvents() {
    sf::Event event;
    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _window.close();
    }
}

void App::update(float time) {
    auto& currentMap = _map.getMap(0);

    if (_player.getRect().left > 200)
        offsetX = _player.getRect().left - 200;

    if (_player.getRect().intersects(_enemy.getRect()) && _enemy.isAlive()) {
        if (_player.getDY() > 0) {
            _enemy.setDX(0);
            _player.setDY(-0.2f);
            _enemy.setAlive(false);
        } else {
            if (!_player.getIsHit()) {
                _player.setIsHit(true);
                _player.getHitClock().restart();
                _player.setFlashCount(0);
            }
        }
    }

    _player.controlPlayer(sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::K, sf::Keyboard::J);
    _player.update(time, currentMap, _window);
    _player.updateBullets(time, currentMap);
    _enemy.update(time, currentMap);
}

void App::render() {
    _window.clear(sf::Color(107, 140, 255));
    _map.render(_window, _tileSet);

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

void App::run() {
    while (_window.isOpen()) {
        float time = _clock.restart().asMicroseconds() / 500.f;
        if (time > 20) time = 20;

        handleEvents();
        update(time);
        render();
    }
}