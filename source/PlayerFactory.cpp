#include "PlayerFactory.h"

Player* ContraFactory::createPlayer() {
    return new Contra();
}

Player* LugciFactory::createPlayer() {
    return new Lugci();
}

void PlayerFactoryManager::registerFactory(PlayerType type, std::unique_ptr<PlayerFactory> factory) {
    _factories[type] = std::move(factory);
}

Player* PlayerFactoryManager::createPlayer(PlayerType type) {
    return _factories.at(type)->createPlayer();
}