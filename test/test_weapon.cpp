#include "doctest.h"
#include "Weapon.h"
#include "Player.h"

TEST_CASE("Weapon::Shoot & Gun behavior") {
    Gun gun;
    CHECK(gun.isActive() == false);

    gun.Shoot(100.f, 50.f, WeaponDirection::Right);
    CHECK(gun.isActive() == true);
    CHECK(gun.getRect().left == doctest::Approx(100.f));
    CHECK(gun.getRect().top == doctest::Approx(50.f));

    // Check direction & speed (right)
    gun.Shoot(0.f, 0.f, WeaponDirection::Right);
    CHECK(gun.isActive() == true);

    // Check direction & speed (left)
    gun.Shoot(0.f, 0.f, WeaponDirection::Left);
    CHECK(gun.isActive() == true);

    // Check direction & speed (up)
    gun.Shoot(0.f, 0.f, WeaponDirection::Up);
    CHECK(gun.isActive() == true);
}

TEST_CASE("Weapon::deactivate and setActive") {
    Gun gun;
    gun.setActive(true);
    CHECK(gun.isActive() == true);
    gun.deactivate();
    CHECK(gun.isActive() == false);
}

TEST_CASE("Weapon::collision with map") {
    Gun gun;
    gun.Shoot(0.f, 0.f, WeaponDirection::Right);
    std::vector<std::string> map(20, "0000000000000000000000");
    map[0][0] = '1';  // tạo va chạm ngay vị trí đầu

    gun.collision(map);
    CHECK(gun.isActive() == false);
}

TEST_CASE("Gun::update position and out of bounds") {
    Gun gun;
    gun.Shoot(10.f, 10.f, WeaponDirection::Right);
    std::vector<std::string> map(100, std::string(100, ' '));

    gun.update(1000.f, map);  // move 1000ms
    CHECK(gun.getRect().left > 10.f);  // di chuyển qua phải

    // Đẩy đạn ra ngoài map
    gun.Shoot(-100.f, -100.f, WeaponDirection::Right);
    gun.update(100.f, map);
    CHECK(gun.isActive() == false);
}

TEST_CASE("Gun::attack cooldown and bullet spawn") {
    Contra player;
    Gun gun;

    gun.attack(player);  // bắn được
    int count1 = player.getBullets().size();
    CHECK(count1 > 0);

    gun.attack(player);  // không bắn được ngay
    int count2 = player.getBullets().size();
    CHECK(count2 == count1);  // không tăng thêm đạn

    // giả lập cooldown chờ 600ms
    sf::sleep(sf::milliseconds(600));
    gun.attack(player);  // bắn tiếp
    int count3 = player.getBullets().size();
    CHECK(count3 > count2);
}
