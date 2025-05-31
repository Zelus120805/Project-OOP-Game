#include "doctest.h"
#include "Enemy.h"
#include "Player.h"

TEST_CASE("SlimeEnemy initialization") {
    Enemy* slime = new SlimeEnemy();
    CHECK(slime != nullptr);
    CHECK(slime->isAlive() == true);
    CHECK(slime->getDamage() == 25.f);
}

TEST_CASE("SlimeEnemy setEnemy sets position and texture rect") {
    Enemy* slime = new SlimeEnemy();
    slime->setEnemy(50, 100);
    auto rect = slime->getRect();
    CHECK(rect.left == 50);
    CHECK(rect.top == 100);
    CHECK(rect.width == 16);
    CHECK(rect.height == 16);
}

TEST_CASE("SlimeEnemy takeDamage reduces hp and triggers die") {
    Enemy* slime = new SlimeEnemy();
    slime->setEnemy(0, 0);
    slime->takeDamage(50);
    CHECK(slime->isAlive() == true);  // HP còn 50
    slime->takeDamage(60);
    CHECK(slime->isAlive() == false); // HP <= 0, chết
}

TEST_CASE("SlimeEnemy attack changes dx direction") {
    Enemy* slime = new SlimeEnemy();
    slime->setEnemy(100, 100);

    // Attack player on right side
    slime->attack(150);
    CHECK(slime->getDX() > 0);

    // Attack player on left side
    slime->attack(50);
    CHECK(slime->getDX() < 0);
}

TEST_CASE("SlimeEnemy updateEnemy basic call") {
    Enemy* slime = new SlimeEnemy();
    slime->setEnemy(100, 100);
    Player* player = new Contra();

    // Fake map, empty vector with some floor at bottom
    std::vector<std::string> tileMap(20, std::string(20, ' '));
    tileMap[19] = std::string(20, '0');  // floor

    // Call updateEnemy multiple times to simulate game loop
    slime->updateEnemy(16, tileMap, *player);
    slime->updateEnemy(16, tileMap, *player);

    // Không crash, vẫn sống
    CHECK(slime->isAlive() == true);
}