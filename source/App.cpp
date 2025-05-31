#include "App.h"

App::App() : _window(sf::VideoMode(450, 300), "Game"), _player1(), _player2() {
    init();
}

App::~App() {
    clearObjects();
}

void App::preparePauseButton() {
    // Pause button (biểu tượng dấu "=" với 2 gạch ngang)
    _pauseButton.setSize(sf::Vector2f(30, 30));
    _pauseButton.setPosition(_window.getSize().x - 40, 10);
    _pauseButton.setFillColor(sf::Color::White);

    // Menu buttons
    _continueText.setFont(_font);
    _continueText.setString("Continue");
    _continueText.setCharacterSize(25);
    _continueText.setPosition((_window.getSize().x - _continueText.getLocalBounds().width) / 2.f, 70);
    _continueText.setFillColor(sf::Color::White);

    _restartText.setFont(_font);
    _restartText.setString("Restart");
    _restartText.setCharacterSize(25);
    _restartText.setPosition((_window.getSize().x - _restartText.getLocalBounds().width) / 2.f, 130);
    _restartText.setFillColor(sf::Color::White);

    _exitText.setFont(_font);
    _exitText.setString("Exit");
    _exitText.setCharacterSize(25);
    _exitText.setPosition((_window.getSize().x - _exitText.getLocalBounds().width) / 2.f, 190);
    _exitText.setFillColor(sf::Color::White);

    // Continue background
    _continueBg.setSize(sf::Vector2f(_continueText.getLocalBounds().width + 20, 40));
    _continueBg.setPosition(_continueText.getPosition().x - 10, _continueText.getPosition().y - 5);
    _continueBg.setFillColor(sf::Color::White);

    // Restart background
    _restartBg.setSize(sf::Vector2f(_restartText.getLocalBounds().width + 20, 40));
    _restartBg.setPosition(_restartText.getPosition().x - 10, _restartText.getPosition().y - 5);
    _restartBg.setFillColor(sf::Color::White);

    // Exit background
    _exitBg.setSize(sf::Vector2f(_exitText.getLocalBounds().width + 20, 40));
    _exitBg.setPosition(_exitText.getPosition().x - 10, _exitText.getPosition().y - 5);
    _exitBg.setFillColor(sf::Color::White);

    // Đặt màu chữ đen để nổi bật
    _continueText.setFillColor(sf::Color::Black);
    _restartText.setFillColor(sf::Color::Black);
    _exitText.setFillColor(sf::Color::Black);
}

void App::prepareGameOver() {
    if (!ResourceLoader::loadTexture(_gameOverTexture, "Tiles/Assets/GameOver.png"))
        exit(1);

    _gameOverSprite.setTexture(_gameOverTexture);
    float scaleX = 350.f / _gameOverTexture.getSize().x;
    float scaleY = 200.f / _gameOverTexture.getSize().y;
    _gameOverSprite.setScale(scaleX, scaleY);
    _gameOverSprite.setPosition(_window.getSize().x / 2.f - _gameOverSprite.getGlobalBounds().width / 2.f,
                        _window.getSize().y / 2.f - _gameOverSprite.getGlobalBounds().height / 2.f - 10);

    // Restart background
    _restartLose.setSize(sf::Vector2f(205, 25));
    _restartLose.setPosition(122, 177);

    // Exit background
    _exitLose.setSize(sf::Vector2f(150, 20));
    _exitLose.setPosition(150, 210);
}

void App::init() {
    _player1 = nullptr;
    _player2 = nullptr;
    _enemy.clear();

    _gameState = GameState::MenuMain;

    if (!ResourceLoader::loadFont(_font, "Font/04B_30__.ttf"))
        exit(1);

    if (!ResourceLoader::loadFont(_fontTime,"Font/CHICKEN Pie.ttf"))
        exit(1);

    if (!ResourceLoader::loadTexture(_mainMenu,"Tiles/Assets/BackgroundMainMenu.png"))
        exit(1);
    
    _backgroundMainMenu.setTexture(_mainMenu);
    float scaleX = 450.f / _mainMenu.getSize().x;
    float scaleY = 300.f / _mainMenu.getSize().y;
    _backgroundMainMenu.setScale(scaleX, scaleY);

    // Main Menu
    _1PlayerButton.setSize(sf::Vector2f(224, 45));
    _1PlayerButton.setPosition(112, 30);

    _2PlayersButton.setSize(sf::Vector2f(224, 45));
    _2PlayersButton.setPosition(112, 92);

    _optionsButton.setSize(sf::Vector2f(170, 45));
    _optionsButton.setPosition(140, 150);

    _exitButton.setSize(sf::Vector2f(120, 45));
    _exitButton.setPosition(165, 209);
    
    // Timer
    _timerText.setFont(_fontTime);
    _timerText.setCharacterSize(15);
    _timerText.setFillColor(sf::Color::Red);
    _timerText.setPosition(_window.getSize().x / 2.f - 25, 10);

    preparePauseButton();
    prepareGameOver();
}

sf::Vector2f App::calculateMidpoint(const sf::FloatRect& r1, const sf::FloatRect& r2) {
    float cx = (r1.left + r1.width / 2 + r2.left + r2.width / 2) / 2;
    float cy = (r1.top + r1.height / 2 + r2.top + r2.height / 2) / 2;
    return {cx, cy};
}

void App::updateCameraOffset(const sf::FloatRect& rect) {
    if (rect.left > 200) 
        offsetX = rect.left - 200;
    if (rect.top > 270) 
        offsetY = rect.top - 270;
}

void App::renderBullets(Player* player) {
    for (const auto& b : player->getBullets()) {
        if (b->isActive()) {
            auto& sprite = player->getBulletSprite();
            sprite.setPosition(b->getPosition());
            _window.draw(sprite);
        }
    }
}

void App::drawUIGame() {
    // 1. Nút pause hình "="
    sf::RectangleShape bar1(sf::Vector2f(4, 20));
    sf::RectangleShape bar2(sf::Vector2f(4, 20));
    bar1.setPosition(_pauseButton.getPosition().x + 6, _pauseButton.getPosition().y + 5);
    bar2.setPosition(_pauseButton.getPosition().x + 18, _pauseButton.getPosition().y + 5);
    bar1.setFillColor(sf::Color::Black);
    bar2.setFillColor(sf::Color::Black);

    if (_gameState == GameState::Playing) {
        _gameTime = _gameClock.getElapsedTime() + _pausedTime;
    }
    std::stringstream ss;
    ss << "Time: " << std::fixed << std::setprecision(1) << _gameTime.asSeconds() << " s";
    _timerText.setString(ss.str());

    // Vẽ các đối tượng
    _window.draw(_pauseButton);

    _window.draw(_timerText);

    _window.draw(bar1);
    _window.draw(bar2);
}

void App::handleEvents() {
    sf::Event event;
    while (_window.pollEvent(event)) {
        sf::Vector2f mousePos = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));

        _continueBg.setFillColor(_continueBg.getGlobalBounds().contains(mousePos) ? sf::Color(200, 200, 200) : sf::Color::White);
        _restartBg.setFillColor(_restartBg.getGlobalBounds().contains(mousePos) ? sf::Color(200, 200, 200) : sf::Color::White);
        _exitBg.setFillColor(_exitBg.getGlobalBounds().contains(mousePos) ? sf::Color(200, 200, 200) : sf::Color::White);

        if (event.type == sf::Event::Closed)
            _window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            mousePos = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));

            // Nhấn nút Pause
            if (_gameState == GameState::Playing) {
                if (_pauseButton.getGlobalBounds().contains(mousePos)) {
                    _gameState = GameState::Paused;
                    _pausedTime = _gameTime;
                }
            }

            // Nếu đang Pause, kiểm tra các nút menu
            if (_gameState == GameState::Paused) {
                if (_continueBg.getGlobalBounds().contains(mousePos)) {
                    _gameState = GameState::Playing;
                    _gameClock.restart();
                }

                if (_restartBg.getGlobalBounds().contains(mousePos)) {
                    _gameState = GameState::Restarting;
                }

                if (_exitBg.getGlobalBounds().contains(mousePos)) {
                    _gameState = GameState::Exiting;
                }
            }

            if (_gameState == GameState::Lost) {
                if (_restartLose.getGlobalBounds().contains(mousePos)) {
                    _gameState = GameState::Restarting;
                }

                if (_exitLose.getGlobalBounds().contains(mousePos)) {
                    _gameState = GameState::Exiting;
                }
            }
        }
    }
}

void App::update(float time, const std::vector<std::string>& currentMap) {
    if (_gameState == GameState::Paused || _gameState == GameState::Lost) 
        return;

    if (_2Players) {
        if (!_player1->finishPlayer() && !_player2->finishPlayer()) {
            auto center = calculateMidpoint(_player1->getRect(), _player2->getRect());
            if (center.x > 200) 
                offsetX = center.x - 200;
            if (center.y > 270) 
                offsetY = center.y - 270;
        } else if (!_player1->finishPlayer()) {
            updateCameraOffset(_player1->getRect());
        } else if (!_player2->finishPlayer()) {
            updateCameraOffset(_player2->getRect());
        } else {
            _gameState = GameState::Lost;
        }
    } else {
        if (!_player1->finishPlayer()) {
            updateCameraOffset(_player1->getRect());
        } else {
            _gameState = GameState::Lost;
        }
    }

    playerCollisionWithEnemy();
    bulletCollisionWithEnemy();

    _player1->controlPlayer(_player1Keys);
    _player1->update(time, currentMap, _window);
    _player1->updateWeapons(time, currentMap);

    if (_2Players) {
        _player2->controlPlayer(_player2Keys);
        _player2->update(time, currentMap, _window);
        _player2->updateWeapons(time, currentMap);
    }

    for (auto e : _enemy) {
        e->updateEnemy(time, currentMap, *_player1);
        if (_2Players) 
            e->updateEnemy(time, currentMap, *_player2);
    }
}

void App::render() {
    _window.clear(sf::Color(107, 198, 255));
    _map.render(_window, _tileSet);

    renderBullets(_player1);
    if (_2Players) 
        renderBullets(_player2);

    drawUIGame();

    _window.draw(_player1->getPlayerSprite());
    _window.draw(_hpPlayer1Text);
    drawHPBar(*_player1, {70, 15});

    if (_2Players) {
        _window.draw(_player2->getPlayerSprite());
        _window.draw(_hpPlayer2Text);
        drawHPBar(*_player2, {70, 30});
    }

    for (auto& e : _enemy) {
        _window.draw(e->getSprite());
    }

    if (_gameState == GameState::Paused) 
        drawPauseMenu();
    if (_gameState == GameState::Lost) 
        drawGameOver();

    if (isWinGame()) {
        _gameState = GameState::Paused;
    }

    _window.display();
}

bool App::isWinGame() const {
    // Kiểm tra nếu tất cả kẻ địch đều đã chết
    for (const auto& e : _enemy) {
        if (e->isAlive()) 
            return false;
    }
    return true;
}

void App::playerCollisionWithEnemy() {
    for (auto& e : _enemy) {
        if (!e->isAlive()) 
            continue;

        // Kiểm tra va chạm với player1
        if (_player1->getRect().intersects(e->getRect())) {
            if (_player1->getDY() > 0) {
                e->setDX(0);
                _player1->setDY(-0.2f);
                e->takeDamage(100.f);
            } else {
                if (!_player1->getIsHit()) {
                    _player1->setIsHit(true, e->getDamage());
                    _player1->getHitClock().restart();
                }
            }
        }

        // Kiểm tra va chạm với player2 nếu có
        if (_2Players && _player2->getRect().intersects(e->getRect())) {
            if (_player2->getDY() > 0) {
                e->setDX(0);
                _player2->setDY(-0.2f);
                e->takeDamage(100.f);
            } else {
                if (!_player2->getIsHit()) {
                    _player2->setIsHit(true, e->getDamage());
                    _player2->getHitClock().restart();
                }
            }
        }
    }
}

void App::bulletCollisionWithEnemy() {
    for (auto& b : _player1->getBullets()) {
        if (!b->isActive()) continue;

        for (auto& e : _enemy) {
            if (b->getRect().intersects(e->getRect()) && e->isAlive()) {
                b->setActive(false);        // Ẩn viên đạn
                e->takeDamage(b->getDamage());
            }
        }
    }

    if (!_2Players) return; // Không kiểm tra va chạm nếu chỉ có 1 người chơi

    for (auto& b : _player2->getBullets()) {
        if (!b->isActive()) continue;

        for (auto& e : _enemy) {
            if (b->getRect().intersects(e->getRect()) && e->isAlive()) {
                b->setActive(false);        // Ẩn viên đạn
                e->takeDamage(b->getDamage());
            }
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

    if (ratio > 0)
        _window.draw(bar);
    else {
        // Nếu HP <= 0, vẽ thanh HP màu đỏ
        bar.setFillColor(sf::Color::Red);
        _window.draw(bar);

        // Vẽ chữ "DEAD" màu đỏ đè lên thanh máu
        sf::Text deadText;
        deadText.setFont(_font);  // Giả sử bạn đã load sẵn font vào biến _font
        deadText.setString("DEAD");
        deadText.setCharacterSize(10);
        deadText.setFillColor(sf::Color::Red);

        // Căn giữa chữ DEAD với thanh máu
        sf::FloatRect textBounds = deadText.getLocalBounds();
        deadText.setPosition(
            position.x + (100 - textBounds.width) / 2,
            position.y - 2 // hơi cao hơn để nhìn rõ
        );

        _window.draw(deadText);
    }
}

void App::drawPauseMenu() {
    // Nền mờ khi pause
    sf::RectangleShape overlay(sf::Vector2f(_window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    _window.draw(overlay);

    // Hiển thị menu
    _window.draw(_continueBg);
    _window.draw(_continueText);

    _window.draw(_restartBg);
    _window.draw(_restartText);

    _window.draw(_exitBg);
    _window.draw(_exitText);
}

void App::drawGameOver() {
    // Nền mờ khi GameOver
    sf::RectangleShape overlay(sf::Vector2f(_window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    _window.draw(overlay);

    // Hiển thị Game Over
    _window.draw(_gameOverSprite);
}

void App::run() {
    init();

    auto& level = _map.getMap(0);

    while (_window.isOpen()) {
        if (_gameState == GameState::MenuMain) {
            _music.stop();

            sf::Event event;
            while (_window.pollEvent(event)) {
                sf::Vector2f mousePos = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));

                if (event.type == sf::Event::Closed)
                    _window.close();

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    mousePos = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));

                    if (_1PlayerButton.getGlobalBounds().contains(mousePos)) {
                        _2Players = false; // Mặc định là chơi 1 người
                        _gameState = GameState::Playing;
                    }

                    if (_2PlayersButton.getGlobalBounds().contains(mousePos)) {
                        _2Players = true; // Chơi 2 người
                        _gameState = GameState::Playing;
                    }

                    if (_optionsButton.getGlobalBounds().contains(mousePos)) {
                        std::cout << "Options\n";
                    }

                    if (_exitButton.getGlobalBounds().contains(mousePos)) {
                        return;
                    }
                }
            }

            _window.clear();
            _window.draw(_backgroundMainMenu);
            _window.display();
        }
        
        if (_gameState == GameState::Playing) {
            runGame(level);
            if (_gameState == GameState::Restarting) {
                _gameState = GameState::Playing;
            }

            if (_gameState == GameState::Exiting) {
                _gameState = GameState::MenuMain;
            }
        }
    }
}

void App::initGame() {
    // Xoá các đối tượng cũ nếu có
    clearObjects();
    _player1 = new Contra();
    _player1Keys[PlayerAction::Left]  = sf::Keyboard::A;
    _player1Keys[PlayerAction::Right] = sf::Keyboard::D;
    _player1Keys[PlayerAction::Down]  = sf::Keyboard::S;
    _player1Keys[PlayerAction::Up]    = sf::Keyboard::W;
    _player1Keys[PlayerAction::Fire]  = sf::Keyboard::J;
    _player1Keys[PlayerAction::Jump]  = sf::Keyboard::K;

    if (_2Players) {
        _player2 = new Lugci();
        _player2Keys[PlayerAction::Left]  = sf::Keyboard::Left;
        _player2Keys[PlayerAction::Right] = sf::Keyboard::Right;
        _player2Keys[PlayerAction::Down]  = sf::Keyboard::Down;
        _player2Keys[PlayerAction::Up]    = sf::Keyboard::Up;
        _player2Keys[PlayerAction::Fire]  = sf::Keyboard::Num2;
        _player2Keys[PlayerAction::Jump]  = sf::Keyboard::Num1;
    }
    _enemy.push_back(new SlimeEnemy());
    _enemy.push_back(new SlimeEnemy());

    
    offsetX = 0;
    offsetY = 0;

    // Time
    _gameClock.restart();
    _pausedTime = sf::Time::Zero;

    if (!_map.loadBackground("Tiles/Assets/Background_2.png")) {
        std::cerr << "Failed to load background\n";
    }

    if (!ResourceLoader::loadTexture(_tileSet, "Tiles/Assets/Assets.png"))
        exit(1);

    _player1->setPlayer(90, 120);
    if (_2Players)
        _player2->setPlayer(120, 120);

    // for (auto e : _enemy)
    //     e->setEnemy(800, 150);
    _enemy[0]->setEnemy(800, 150);
    _enemy[1]->setEnemy(700, 150);

    if (!ResourceLoader::loadSound(_music, "Sound/Mario_Theme.ogg"))
        exit(1);

    _hpPlayer1Text.setFont(_font);
    _hpPlayer1Text.setString("Player 1");
    _hpPlayer1Text.setCharacterSize(8);
    _hpPlayer1Text.setFillColor(sf::Color::Black);
    _hpPlayer1Text.setPosition(10, 15);

    _hpPlayer2Text.setFont(_font);
    _hpPlayer2Text.setString("Player 2");
    _hpPlayer2Text.setCharacterSize(8);
    _hpPlayer2Text.setFillColor(sf::Color::Black);
    _hpPlayer2Text.setPosition(10, 30);

    _music.setLoop(true);
    _music.play();
}

void App::clearObjects() {
    // std::cout << "Clear Objects...\n";
    if (_player1)
        delete _player1;
    if (_player2)
        delete _player2;

    for (auto e : _enemy) {
        delete e;
    }
    _enemy.clear();
        
    _player1 = nullptr;
    _player2 = nullptr;
}

void App::runGame(const std::vector<std::string>& level) {
    initGame();
    while (_window.isOpen()) {
        float time = _clock.restart().asMicroseconds() / 500.f;
        if (time > 20) 
            time = 20;

        handleEvents();
        if (_gameState == GameState::Restarting || _gameState == GameState::Exiting)
           return;
        update(time, level);
        render();
    }
}