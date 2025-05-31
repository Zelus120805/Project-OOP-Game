// PlayerFactory.h
#ifndef _PLAYERFACTORY_H_
#define _PLAYERFACTORY_H_

#include "Player.h" // để có Contra, Lugci

class PlayerFactory {
public:
    virtual ~PlayerFactory() {}
    virtual Player* createPlayer() = 0;
};

class ContraFactory : public PlayerFactory {
public:
    Player* createPlayer() override;
};

class LugciFactory : public PlayerFactory{
public:
    Player* createPlayer() override;
};

class PlayerFactoryManager {
private:
    std::map<PlayerType, std::unique_ptr<PlayerFactory>> _factories;

public:
    void registerFactory(PlayerType type, std::unique_ptr<PlayerFactory> factory);
    Player* createPlayer(PlayerType type);
};

#endif