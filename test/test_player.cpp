#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Player.h"

class TestPlayer : public Player {
public:
    void setPlayer(float x, float y) override {
        rect = sf::FloatRect(x, y, 24, 35);
        _hp = 100.f;
        _hpPlayer = 100.f;
        _isDamagedTaken = false;
        isHit = false;
        _damageEnemy = 0.f;
    }
    void update(float, const std::vector<std::string>&, sf::RenderWindow&) override {}
    void setSpriteByPose(PlayerPose, float) override {}
};

TEST_CASE("Player getter/setter basics") {
    TestPlayer p;
    p.setPlayer(10, 20);

    CHECK(p.getRect().left == 10);
    CHECK(p.getRect().top == 20);
    CHECK(p.getHP() == 100.f);
    CHECK(p.getHPPlayer() == 100.f);

    p.setDY(5.5f);
    CHECK(p.getDY() == doctest::Approx(5.5f));

    p.setIsHit(true, 25.f);
    CHECK(p.getIsHit() == true);
}

TEST_CASE("Player takeDamage works correctly") {
    TestPlayer p;
    p.setPlayer(0, 0);

    // Giả sử enemy gây 30 damage
    p.setIsHit(true, 30.f);

    // Gọi hàm takeDamage sẽ trừ HP
    p.takeDamage();

    // Sau 1 lần damage, _hp phải giảm 30
    CHECK(p.getHP() == doctest::Approx(70.f));

    // Gọi lại takeDamage trước khi reset thì HP không giảm nữa
    p.takeDamage();
    CHECK(p.getHP() == doctest::Approx(70.f));

    p.setIsHit(true, 80.f);

    p.takeDamage();
    CHECK(p.getHP() == doctest::Approx(0.f));
}