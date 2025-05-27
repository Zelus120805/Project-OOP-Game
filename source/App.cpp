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
    if (!_gameOverTexture.loadFromFile("Tiles/Assets/GameOver.png")) {
        std::cout << "Không thể tải file GameOver.\n";
        exit(1);
    }

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
    _enemy = nullptr;

    _isPlaying = false;
    _isPaused = false;
    _isRestart = false;
    _isExit = false;

    if (!_font.loadFromFile("Font/04B_30__.ttf")) {
        std::cout << "Không thể tải font.\n";
        exit(1);
    }

    if (!_fontTime.loadFromFile("Font/CHICKEN Pie.ttf")) {
        std::cout << "Không thể tải font.\n";
        exit(1);
    }

    if (!_mainMenu.loadFromFile("Tiles/Assets/BackgroundMainMenu.png")) {
        std::cout << "Không thể tải file.\n";
        exit(1);
    }
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
            if (!_isPaused) {
                if (_pauseButton.getGlobalBounds().contains(mousePos)) {
                    _isPaused = true;
                    _pausedTime = _gameTime;
                }
            }

            // Nếu đang Pause, kiểm tra các nút menu
            if (_isPaused) {
                if (_continueBg.getGlobalBounds().contains(mousePos)) {
                    _isPaused = false;
                    _gameClock.restart();
                }

                if (_restartBg.getGlobalBounds().contains(mousePos)) {
                    _isPaused = false;
                    _isRestart = true;
                }

                if (_exitBg.getGlobalBounds().contains(mousePos)) {
                    _isPaused = false;
                    _isExit = true;
                }
            }

            if (_isLose) {
                if (_restartLose.getGlobalBounds().contains(mousePos)) {
                    _isRestart = true;
                }

                if (_exitLose.getGlobalBounds().contains(mousePos)) {
                    _isExit = true;
                }
            }
        }
    }
}

void App::update(float time, const std::vector<std::string>& currentMap) {
    if (_isPaused || _isLose)
        return;

    if (_2Players) {
        if (!_player1->finishPlayer() && !_player2->finishPlayer()) {
            sf::FloatRect rect1 = _player1->getRect();
            sf::FloatRect rect2 = _player2->getRect();

            // Tính trung điểm
            float centerX = (rect1.left + rect1.width / 2 + rect2.left + rect2.width / 2) / 2;
            float centerY = (rect1.top + rect1.height / 2 + rect2.top + rect2.height / 2) / 2;

            if (centerX > 200)
                offsetX = centerX - 200;
            if (centerY > 300)
                offsetY = centerY - 300;
        }
        else if (!_player1->finishPlayer()) {
            if (_player1->getRect().left > 200)
                offsetX = _player1->getRect().left - 200;

            if (_player1->getRect().top > 300)
                offsetY = _player1->getRect().top - 300;
        }
        else if (!_player2->finishPlayer()) {
            if (_player2->getRect().left > 300)
                offsetX = _player2->getRect().left - 200;

            if (_player2->getRect().top > 250)
                offsetY = _player2->getRect().top - 250;
        }
        else {
            _isLose = true;
        }
    }
    else {
        if (!_player1->finishPlayer()) {
            if (_player1->getRect().left > 200)
                offsetX = _player1->getRect().left - 200;

            if (_player1->getRect().top > 300)
                offsetY = _player1->getRect().top - 300;
        }
        else {
            _isLose = true;
        }
    }

    playerCollisionWithEnemy();
    bulletCollisionWithEnemy();

    _player1->controlPlayer(sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::K, sf::Keyboard::J);
    _player1->update(time, currentMap, _window);
    _player1->updateWeapons(time, currentMap);

    if (_2Players) {
        _player2->controlPlayer(sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Num2, sf::Keyboard::Num1);
        _player2->update(time, currentMap, _window);
        _player2->updateWeapons(time, currentMap);
    }

    _enemy->updateEnemy(time, currentMap, *_player1);
    if (_2Players)
        _enemy->updateEnemy(time, currentMap, *_player2);
}

void App::render() {
    _window.clear(sf::Color(107, 140, 255));
    _map.render(_window, _tileSet);

    for (const auto& b : _player1->getBullets()) {
        if (b->isActive()) {
            _player1->getBulletSprite().setPosition(b->getPosition().x, b->getPosition().y);
            _window.draw(_player1->getBulletSprite());
        }
    }

    if (_2Players) {
        for (const auto& b : _player2->getBullets()) {
            if (b->isActive()) {
                _player2->getBulletSprite().setPosition(b->getPosition().x, b->getPosition().y);
                _window.draw(_player2->getBulletSprite());
            }
        }
    }

    // 1. Nút pause hình "="
    sf::RectangleShape bar1(sf::Vector2f(4, 20));
    sf::RectangleShape bar2(sf::Vector2f(4, 20));
    bar1.setPosition(_pauseButton.getPosition().x + 6, _pauseButton.getPosition().y + 5);
    bar2.setPosition(_pauseButton.getPosition().x + 18, _pauseButton.getPosition().y + 5);
    bar1.setFillColor(sf::Color::Black);
    bar2.setFillColor(sf::Color::Black);

    if (!_isPaused && !_isLose) {
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

    _window.draw(_player1->getPlayerSprite());
    _window.draw(_hpPlayer1Text);
    drawHPBar(*_player1, sf::Vector2f(70, 15));

    if (_2Players) {
        _window.draw(_player2->getPlayerSprite());
        _window.draw(_hpPlayer2Text);
        drawHPBar(*_player2, sf::Vector2f(70, 30));
    }

    _window.draw(_enemy->getSprite());

    // 2. Nếu đang pause thì vẽ menu
    if (_isPaused) {
        drawPauseMenu();
    }

    // 3. Nếu Game Over thì vẽ Game Over
    if (_isLose) {
        drawGameOver();
    }

    _window.display();
}

void App::playerCollisionWithEnemy() {
    if (_player1->getRect().intersects(_enemy->getRect()) && _enemy->isAlive()) {
        if (_player1->getDY() > 0) {
            _enemy->setDX(0);
            _player1->setDY(-0.2f);
            _enemy->takeDamage(100.f);
        } else {
            if (!_player1->getIsHit()) {
                _player1->setIsHit(true, _enemy->getDamage());
                _player1->getHitClock().restart();
            }
        }
    }

    if (!_2Players) return; // Không kiểm tra va chạm nếu chỉ có 1 người chơi

    if (_player2->getRect().intersects(_enemy->getRect()) && _enemy->isAlive()) {
        if (_player2->getDY() > 0) {
            _enemy->setDX(0);
            _player2->setDY(-0.2f);
            _enemy->takeDamage(100.f);
        } else {
            if (!_player2->getIsHit()) {
                _player2->setIsHit(true, _enemy->getDamage());
                _player2->getHitClock().restart();
            }
        }
    }
}

void App::bulletCollisionWithEnemy() {
    for (auto& b : _player1->getBullets()) {
        if (!b->isActive()) continue;

        if (b->getRect().intersects(_enemy->getRect()) && _enemy->isAlive()) {
            b->setActive(false);        // Ẩn viên đạn
            _enemy->takeDamage(b->getDamage());
        }
    }

    if (!_2Players) return; // Không kiểm tra va chạm nếu chỉ có 1 người chơi

    for (auto& b : _player2->getBullets()) {
        if (!b->isActive()) continue;

        if (b->getRect().intersects(_enemy->getRect()) && _enemy->isAlive()) {
            b->setActive(false);        // Ẩn viên đạn
            _enemy->takeDamage(b->getDamage());
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
        if (!_isPlaying) {
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
                        _isPlaying = true;
                    }

                    if (_2PlayersButton.getGlobalBounds().contains(mousePos)) {
                        _2Players = true; // Chơi 2 người
                        _isPlaying = true;
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
        
        if (_isPlaying) {
            runGame(level);
            if (_isRestart)
                _isRestart = false;

            if (_isExit) {
                _isPlaying = false;
                _isExit = false;
            }
        }
    }
}

void App::initGame() {
    // Xoá các đối tượng cũ nếu có
    clearObjects();
    _player1 = new Contra();
    if (_2Players)
        _player2 = new Lugci();
    _enemy = new SlimeEnemy();
    offsetX = 0;
    offsetY = 0;

    // Time
    _gameClock.restart();
    _pausedTime = sf::Time::Zero;

    if (!_map.loadBackground("Tiles/Assets/Background_2.png")) {
        std::cerr << "Failed to load background\n";
    }

    if (!_tileSet.loadFromFile("Tiles/Assets/Assets.png"))
        std::cerr << "Error loading Tiles.png\n";

    _player1->setPlayer(90, 120);
    if (_2Players)
        _player2->setPlayer(120, 120);

    _enemy->setEnemy(800, 150);

    if (!_music.openFromFile("Sound/Mario_Theme.ogg"))
        std::cerr << "Missing Mario_Theme.ogg\n";

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

    _isWin = false;
    _isLose = false;
}

void App::clearObjects() {
    // std::cout << "Clear Objects...\n";
    if (_player1)
        delete _player1;
    if (_player2)
        delete _player2;

    if (_enemy)
        delete _enemy;
        
    _player1 = nullptr;
    _player2 = nullptr;
    _enemy = nullptr;
}

void App::runGame(const std::vector<std::string>& level) {
    initGame();
    while (_isPlaying && _window.isOpen()) {
        float time = _clock.restart().asMicroseconds() / 500.f;
        if (time > 20) 
            time = 20;

        handleEvents();
        if (_isRestart || _isExit)
           return;
        update(time, level);
        render();
    }
}