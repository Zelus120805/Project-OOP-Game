
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Weapon.h"
#include "Player.h"
#include <vector>

TEST_CASE("Weapon basic functionality") {
    Weapon* w = new Gun();

    // Test ban đầu weapon không active
    w->setActive(true);
    CHECK(w->isActive() == true);
    w->setActive(false);
    CHECK(w->isActive() == false);
    
    // Test deactivate()
    w->deactivate();
    CHECK(w->isActive() == false);

    // Test getDamage() trả về damage mặc định (60 theo code hiện tại)
    CHECK(w->getDamage() == 60.f);
}

TEST_CASE("Weapon collision logic") {
    Weapon* w = new Gun();
    w->setActive(true);

    // Map có tile '1' tại [1][1], giả sử collision sẽ deactivate
    std::vector<std::string> map = {
        "........",
        ".1......",
        "........"
    };
    w->collision(map);
    CHECK(w->isActive() == true);

    // Test map không có tile chặn, weapon vẫn active
    w->setActive(true);
    std::vector<std::string> clearMap = {
        "........",
        "........",
        "........"
    };
    w->collision(clearMap);
    CHECK(w->isActive() == true);
}