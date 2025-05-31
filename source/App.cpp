#include "App.h"

App::App() : _window(sf::VideoMode(450, 300), "Game"), _player1(), _player2() {
    init();
}

App::~App() {
    clearObjects();
    for (auto e : _enemy) {
        delete e;
    }
    _enemy.clear();
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
    try {
        if (!_gameOverTexture.loadFromFile("Tiles/Assets/GameOver.png")) {
            throw MyException(111, "Không thể tải GameOver texture");
        }
    } catch (const MyException& e) {
        std::cout << e.what() << '\n';
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

void App::prepareGameWon() {
    try {
        if (!_gameWonTexture.loadFromFile("Tiles/Assets/YouWin.png")) {
            throw MyException(111, "Không thể tải YouWin texture");
        }
    } catch (const MyException& e) {
        std::cout << e.what() << '\n';
        exit(1);
    }

    _gameWonSprite.setTexture(_gameWonTexture);
    float scaleX = 350.f / _gameWonTexture.getSize().x;
    float scaleY = 200.f / _gameWonTexture.getSize().y;
    _gameWonSprite.setScale(scaleX, scaleY);
    _gameWonSprite.setPosition(_window.getSize().x / 2.f - _gameWonSprite.getGlobalBounds().width / 2.f,
                        _window.getSize().y / 2.f - _gameWonSprite.getGlobalBounds().height / 2.f - 10);

    // Next level background
    _nextLevelWin.setSize(sf::Vector2f(216, 20));
    _nextLevelWin.setPosition(117, 148);

    // Restart background
    _restartWin.setSize(sf::Vector2f(216, 20));
    _restartWin.setPosition(117, 178);

    // Exit background
    _exitWin.setSize(sf::Vector2f(160, 17));
    _exitWin.setPosition(144, 206);
}

void App::init() {
    _player1 = nullptr;
    _player2 = nullptr;
    _enemy.clear();

    _gameState = GameState::MenuMain;

    try {
        if (!_font.loadFromFile("Font/04B_30__.ttf")) {
            throw MyException(112, "Không thể tải font 04B_30__.ttf");
        }
    } catch (const MyException& e) {
        std::cout << e.what() << '\n';
        exit(1);
    }

    try {
        if (!_fontTime.loadFromFile("Font/CHICKEN Pie.ttf")) {
            throw MyException(113, "Không thể tải font CHICKEN Pie.ttf");
        }
    } catch (const MyException& e) {
        std::cout << e.what() << '\n';
        exit(1);
    }

    try {
        if (!_mainMenu.loadFromFile("Tiles/Assets/BackgroundMainMenu.png")) {
            throw MyException(113, "Không thể tải background main menu texture");
        }
    } catch (const MyException& e) {
        std::cout << e.what() << '\n';
        exit(1);
    }

    try {
        if (!InputConfig::loadControls("Player/Controls.txt", _player1Keys, _player2Keys)) {
            throw MyException(123, "Không thể mở file Controls.txt để tải cấu hình điều khiển");
        }
    } catch (const MyException& e) {
        std::cout << e.what() << '\n';
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

    // Option text
    _optionText.setFont(_font);  // Đảm bảo _font đã được load thành công
    _optionText.setString("OPTION");
    _optionText.setCharacterSize(30);  // Tăng kích thước lên cho dễ thấy
    _optionText.setFillColor(sf::Color::Red);
    _optionText.setStyle(sf::Text::Bold);  // Thêm in đậm
    _optionText.setPosition((_window.getSize().x - _optionText.getLocalBounds().width) / 2.f, 15);

    preparePauseButton();
    prepareGameOver();
    prepareGameWon();
    InputConfig::initialize();
    _factoryManager.registerFactory(PlayerType::Contra, std::make_unique<ContraFactory>());
    _factoryManager.registerFactory(PlayerType::Lugci, std::make_unique<LugciFactory>());
}

sf::Vector2f App::calculateMidpoint(const sf::FloatRect& r1, const sf::FloatRect& r2) {
    float cx = (r1.left + r1.width / 2 + r2.left + r2.width / 2) / 2;
    float cy = (r1.top + r1.height / 2 + r2.top + r2.height / 2) / 2;
    return {cx, cy};
}

void App::updateCameraOffset(const sf::FloatRect& rect) {
    if (rect.left > 200) 
        offsetX = rect.left - 200;
    if (rect.top > 260) 
        offsetY = rect.top - 260;
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

            if (_gameState == GameState::Won) {
                if (_nextLevelWin.getGlobalBounds().contains(mousePos)) {
                    _gameState = GameState::NextLevel;
                }

                if (_restartWin.getGlobalBounds().contains(mousePos)) {
                    _gameState = GameState::Restarting;
                }

                if (_exitWin.getGlobalBounds().contains(mousePos)) {
                    _gameState = GameState::Exiting;
                }
            }
        }
    }
}

void App::updateCamera() {
    if (_2Players) {
        if (!_player1->finishPlayer() && !_player2->finishPlayer()) {
            auto center = calculateMidpoint(_player1->getRect(), _player2->getRect());
            if (center.x > 200) 
                offsetX = center.x - 200;
            if (center.y > 260) 
                offsetY = center.y - 260;
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
}

void App::updatePlayers(float time, const std::vector<std::string>& currentMap) {
    _player1->controlPlayer(_player1Keys);
    _player1->update(time, currentMap, _window);
    _player1->updateWeapons(time, currentMap);

    if (_2Players) {
        _player2->controlPlayer(_player2Keys);
        _player2->update(time, currentMap, _window);
        _player2->updateWeapons(time, currentMap);
    }
}

void App::updateEnemies(float time, const std::vector<std::string>& currentMap) {
    for (auto e : _enemy) {
        e->updateEnemy(time, currentMap, *_player1);
        if (_2Players) 
            e->updateEnemy(time, currentMap, *_player2);
    }
}

void App::updateCollisions(){
    playerCollisionWithEnemy();
    bulletCollisionWithEnemy();
}

void App::update(float time, const std::vector<std::string>& currentMap) {
    if (_gameState == GameState::Paused || _gameState == GameState::Lost || _gameState == GameState::Won)
        return;

    updateCamera();
    updateCollisions();
    updatePlayers(time, currentMap);
    updateEnemies(time, currentMap);
}

void App::renderEnemies() {
    for (const auto& e : _enemy) {
        _window.draw(e->getSprite());
    }
}

void App::renderBulletsPlayer() {
    renderBullets(_player1);
    if (_2Players) 
        renderBullets(_player2);
}

void App::renderPlayerAndHp() {
    _window.draw(_player1->getPlayerSprite());
    _window.draw(_hpPlayer1Text);
    drawHPBar(*_player1, {70, 15});

    if (_2Players) {
        _window.draw(_player2->getPlayerSprite());
        _window.draw(_hpPlayer2Text);
        drawHPBar(*_player2, {70, 30});
    }
}

void App::render() {
    _window.clear(sf::Color(107, 140, 255));

    _map.render(_window, _tileSet);

    renderBulletsPlayer();
    drawUIGame();
    renderPlayerAndHp();
    renderEnemies();

    if (_gameState == GameState::Paused) 
        drawPauseMenu();

    if (_gameState == GameState::Lost) 
        drawGameOver();

    if (_gameState == GameState::Won)
        drawGameWon();
    else {
        if (isWinGame()) {
            _gameState = GameState::Won;
        }
    }

    _window.display();
}

bool App::isWinGame() const {
    // Kiểm tra nếu tất cả kẻ địch đều đã chết
    for (const auto& e : _enemy) {
        if (e->isAlive() || !e->isCompleteDie()) 
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

    if (!_2Players) 
        return; // Không kiểm tra va chạm nếu chỉ có 1 người chơi

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

void App::drawGameWon() {
    // Nền mờ khi GameWon
    sf::RectangleShape overlay(sf::Vector2f(_window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    _window.draw(overlay);

    // Hiển thị You Win
    _window.draw(_gameWonSprite);
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
                        _gameState = GameState::Options;
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
        
        if (_gameState == GameState::Playing || _gameState == GameState::Restarting || _gameState == GameState::NextLevel) {
            runGame(level);

            if (_gameState == GameState::Exiting) {
                _gameState = GameState::MenuMain;
            }
        }

        if (_gameState == GameState::Options) {
            menuOptions();
            _gameState = GameState::MenuMain; // Quay lại menu chính sau khi chọn xong
        }
    }
}

void App::initGame() {
    // Xoá các đối tượng cũ nếu có
    clearObjects();

    _player1 = _factoryManager.createPlayer(PlayerType::Contra);
    if (_2Players)
        _player2 = _factoryManager.createPlayer(PlayerType::Lugci);
    
    offsetX = 0;
    offsetY = 0;

    // Time
    _gameClock.restart();
    _pausedTime = sf::Time::Zero;

    try {
        if (!_map.loadBackground("Tiles/Assets/Background_2.png")) {
            throw MyException(114, "Không thể tải background map");
        }
    } catch (const MyException& e) {
        std::cout << e.what() << '\n';
        exit(1);
    }

    try {
        if (!_tileSet.loadFromFile("Tiles/Assets/Assets.png")) {
            throw MyException(115, "Không thể tải Assets.png");
        }
    } catch (const MyException& e) {
        std::cout << e.what() << '\n';
        exit(1);
    }

    try {
        if (!_music.openFromFile("Sound/Mario_Theme.ogg")) {
            throw MyException(115, "Không thể tải nhạc nền Mario_Theme.ogg");
        }
    } catch (const MyException& e) {
        std::cout << e.what() << '\n';
        exit(1);
    }

    spoilPlayer();
    spoilEnemy();
    _gameState = GameState::Playing;

    _music.setLoop(true);
    _music.play();
}

void App::spoilPlayer() {
    _player1->setPlayer(90, 120);
    _hpPlayer1Text.setFont(_font);
    _hpPlayer1Text.setString("Player 1");
    _hpPlayer1Text.setCharacterSize(8);
    _hpPlayer1Text.setFillColor(sf::Color::Black);
    _hpPlayer1Text.setPosition(10, 15);

    if (_2Players) {
        _player2->setPlayer(120, 120);
        _hpPlayer2Text.setFont(_font);
        _hpPlayer2Text.setString("Player 2");
        _hpPlayer2Text.setCharacterSize(8);
        _hpPlayer2Text.setFillColor(sf::Color::Black);
        _hpPlayer2Text.setPosition(10, 30);
    }
}

void App::spoilEnemy() {
    if (_gameState == GameState::NextLevel || _gameState == GameState::Playing) {
        for (auto e : _enemy) {
            delete e;  // Xoá các đối tượng cũ
        }
        _enemy.clear();

        // Tạo ngẫu nhiên số lượng kẻ địch
        int countEnemy = Random::getRandomInt(10, 20);
        for (int i = 0; i < countEnemy; ++i) {
            _enemy.push_back(new SlimeEnemy(Random::getRandomInt(300, 2200), 150));
        }
    }
    else if (_gameState == GameState::Restarting) {
        for (int i = 0; i < _enemy.size(); ++i) {
            int posX = _enemy[i]->getXPos();
            int posY = _enemy[i]->getYPos();
            delete _enemy[i];  // Xoá đối tượng cũ
            _enemy[i] = new SlimeEnemy(posX, posY);
        }
    }
}

void App::clearObjects() {
    // std::cout << "Clear Objects...\n";
    if (_player1)
        delete _player1;
    if (_player2)
        delete _player2;
        
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
        if (_gameState == GameState::Restarting || _gameState == GameState::Exiting || _gameState == GameState::NextLevel)
           return;
        update(time, level);
        render();
    }
}

void App::menuOptions() {
    const std::vector<std::string> actionNames = {
        "Left", "Right", "Up", "Down", "Shoot", "Jump"
    };

    struct ControlItem {
        sf::Text text;
        PlayerAction action;
        bool isPlayer1;
        sf::FloatRect getBounds() const {
            return text.getGlobalBounds();
        }
    };

    std::vector<ControlItem> items;
    for (int i = 0; i < 6; ++i) {
        ControlItem p1;
        p1.text = sf::Text(actionNames[i] + " = " + InputConfig::keyToString(_player1Keys[(PlayerAction)i]), _font, 12);
        p1.text.setPosition(60, 100 + i * 30);
        p1.action = static_cast<PlayerAction>(i);
        p1.isPlayer1 = true;
        items.push_back(p1);

        ControlItem p2;
        p2.text = sf::Text(actionNames[i] + " = " + InputConfig::keyToString(_player2Keys[(PlayerAction)i]), _font, 12);
        p2.text.setPosition(270, 100 + i * 30);
        p2.action = static_cast<PlayerAction>(i);
        p2.isPlayer1 = false;
        items.push_back(p2);
    }

    // --- Thêm tiêu đề Player 1 và Player 2 ---
    sf::Text player1Title("Player 1", _font, 20);
    player1Title.setFillColor(sf::Color::Black);
    player1Title.setPosition(60, 60);

    sf::Text player2Title("Player 2", _font, 20);
    player2Title.setFillColor(sf::Color::Black);
    player2Title.setPosition(270, 60);

    int selectedIndex = -1;

    while (_window.isOpen()) {
        _window.clear(sf::Color(107, 140, 255));

        sf::Event event;
        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                _window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mouseWorld = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));
                for (int i = 0; i < (int)items.size(); ++i) {
                    if (items[i].getBounds().contains(mouseWorld)) {
                        selectedIndex = i;
                        break;
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed && selectedIndex != -1) {
                auto& item = items[selectedIndex];
                if (item.isPlayer1)
                    _player1Keys[item.action] = event.key.code;
                else
                    _player2Keys[item.action] = event.key.code;

                // Cập nhật lại nội dung hiển thị
                item.text.setString(actionNames[(int)item.action] + " = " + InputConfig::keyToString(event.key.code));
                selectedIndex = -1;
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                InputConfig::saveControls("Player/Controls.txt", _player1Keys, _player2Keys);
                return;
            }
        }

        // Tô màu các dòng
        sf::Vector2f mouseWorld = _window.mapPixelToCoords(sf::Mouse::getPosition(_window));
        for (int i = 0; i < (int)items.size(); ++i) {
            if (items[i].getBounds().contains(mouseWorld))
                items[i].text.setFillColor(sf::Color::Yellow);
            else
                items[i].text.setFillColor(sf::Color::White);
            _window.draw(items[i].text);
        }

        // Vẽ tiêu đề
        _window.draw(player1Title);
        _window.draw(player2Title);
        _window.draw(_optionText);
        _window.display();
    }
}