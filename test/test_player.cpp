#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Player.h"

TEST_CASE("Test setter/getter của Player") {
    Player* player = new Contra();
    player->setDY(5.0f);
    CHECK(player->getDY() == doctest::Approx(5.0f));
    
    player->setIsHit(true, 50.f);
    CHECK(player->getIsHit() == true);
}
TEST_CASE("Test HP ban đầu của Player") {
    Player* player = new Lugci();
    CHECK(player->getHP() == player->getHPPlayer());  // Kiểm tra HP ban đầu
}
TEST_CASE("Test giảm HP khi dính damage") {
    Player* player = new Contra();
    player->setIsHit(true, 30.f);
    player->takeDamage();
    CHECK(player->getHP() < player->getHPPlayer());  // HP giảm so với ban đầu
}