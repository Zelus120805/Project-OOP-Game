#include "App.h"

App::App() : _window(sf::VideoMode(450, 300), "Game"), _contra() {
    init();
}

App::~App() {}

void App::init() {   
    if (!_map.loadBackground("Tiles/Assets/Background_3.png")) {
        std::cerr << "Failed to load background\n";
    }

    if (!_tileSet.loadFromFile("Tiles/Assets/Assets.png"))
        std::cerr << "Error loading Tiles.png\n";

    _contra.setPlayer(120, 120);

    //_enemy.setEnemy(_tileSet, 800, 150);

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

    if (_contra.getRect().left > 200)
        offsetX = _contra.getRect().left - 200;

    playerCollisionWithEnemy();
    bulletCollisionWithEnemy();

    _contra.controlPlayer(sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::K, sf::Keyboard::J);
    _contra.update(time, currentMap, _window);
    _contra.updateBullets(time, currentMap);
    //_enemy.update(time, currentMap);
}

void App::render() {
    // _window.clear(sf::Color(107, 140, 255));
    _map.render(_window, _tileSet);

    _window.draw(_contra.getPlayerSprite());
    drawHPBar(_contra, sf::Vector2f(20, 15));

    for (const auto& b : _contra.getBullets()) {
        if (b->isActive()) {
            _contra.getBulletSprite().setPosition(b->getPosition().x, b->getPosition().y);
            _window.draw(_contra.getBulletSprite());
        }
    }

    _window.draw(_enemy.getSprite());
    _window.display();
}

void App::playerCollisionWithEnemy() {
    if (_contra.getRect().intersects(_enemy.getRect()) && _enemy.isAlive()) {
        if (_contra.getDY() > 0) {
            _enemy.setDX(0);
            _contra.setDY(-0.2f);
            _enemy.setAlive(false);
        } else {
            if (!_contra.getIsHit()) {
                _contra.setIsHit(true);
                _contra.getHitClock().restart();
                _contra.setFlashCount(0);
            }
        }
    }
}

void App::bulletCollisionWithEnemy() {
    for (auto& b : _contra.getBullets()) {
        if (!b->isActive()) continue;

        if (b->getRect().intersects(_enemy.getRect()) && _enemy.isAlive()) {
            b->setActive(false);        // Ẩn viên đạn
            _enemy.takeDamage(b->getDamage());
        }
    }
}

void App::drawHPBar(const Player& player, sf::Vector2f position) {
    float ratio = player.getHP() / player.getHPPlayer();

    sf::RectangleShape bg(sf::Vector2f(100, 8));
    bg.setFillColor(sf::Color(50, 50, 50));
    bg.setPosition(position);

    sf::RectangleShape bar(sf::Vector2f(100 * ratio, 8));
    bar.setFillColor(sf::Color::Green);
    bar.setPosition(position);

    _window.draw(bg);
    _window.draw(bar);
}

void App::run() {
    while (_window.isOpen()) {
        float time = _clock.restart().asMicroseconds() / 500.f;
        if (time > 20) 
            time = 20;

        handleEvents();
        update(time);
        render();
    }
}