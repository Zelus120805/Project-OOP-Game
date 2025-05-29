#ifndef _APP_H_
#define _APP_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "Error.h"

enum class GameState {
    MenuMain,
    Playing,
    Options,
    Paused,
    Won,
    Lost,
    Restarting,
    Exiting,
};

class App {
private:
    sf::RenderWindow _window;
    sf::Texture _tileSet;
    sf::Texture _enemyTileSet;

    Map _map;
    Player* _player1;
    std::map<PlayerAction, sf::Keyboard::Key> _player1Keys;
    Player* _player2;
    std::map<PlayerAction, sf::Keyboard::Key> _player2Keys;
    std::vector<Enemy*> _enemy;

    sf::Music _music;
    sf::Clock _clock;

    // Pause menu
    sf::RectangleShape _pauseButton;
    sf::RectangleShape _continueBg, _restartBg, _exitBg;
    sf::RectangleShape _restartLose, _exitLose;
    sf::RectangleShape _1PlayerButton;
    sf::RectangleShape _2PlayersButton;
    sf::RectangleShape _optionsButton;
    sf::RectangleShape _exitButton;
    sf::Texture _mainMenu;
    sf::Texture _gameOverTexture;
    sf::Sprite _backgroundMainMenu;
    sf::Sprite _gameOverSprite;
    sf::Text _continueText, _restartText, _exitText;
    sf::Text _timerText;
    sf::Text _hpPlayer1Text;
    sf::Text _hpPlayer2Text;
    sf::Clock _gameClock;
    sf::Time _gameTime;
    sf::Time _pausedTime;

    // Game
    sf::Font _font;
    sf::Font _fontTime;

    // 2 Players mode
    bool _2Players;
    // Game state
    GameState _gameState = GameState::Playing;

    void preparePauseButton();
    void prepareGameOver();

    void init();
    void handleEvents();
    void update(float time, const std::vector<std::string>& currentMap);
    void render();
    bool isWinGame() const;
private:
    void playerCollisionWithEnemy();
    void bulletCollisionWithEnemy();
    void drawHPBar(const Player& player, sf::Vector2f position);
    void drawPauseMenu();
    void drawGameOver();
    void drawUIGame();
public:
    App();
    virtual ~App();

    sf::Vector2f calculateMidpoint(const sf::FloatRect& r1, const sf::FloatRect& r2);
    void updateCameraOffset(const sf::FloatRect& rect);
    void renderBullets(Player* player);

    void run();
    void clearObjects();
    void initGame();
    void runGame(const std::vector<std::string>& level);
};

#endif // _APP_H_