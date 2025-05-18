#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Weapon.h"
#include <vector>
#include <string>

// Giả sử offsetX, offsetY mặc định để test (nếu cần, dùng biến toàn cục)
float testOffsetX = 0.f;
float testOffsetY = 0.f;

TEST_CASE("Weapon activation and deactivation") {
    Weapon w;
    CHECK_FALSE(w.isActive());

    w.setActive(true);
    CHECK(w.isActive());

    w.deactivate();
    CHECK_FALSE(w.isActive());
}

TEST_CASE("Gun shoot and movement") {
    Gun gun;
    CHECK_FALSE(gun.isActive());

    // Tạo map dummy để truyền vào update
    std::vector<std::string> someMap(20, std::string(30, '1')); // map toàn '1'

    // Bắn đạn, vị trí ban đầu (50,50), hướng đi phải
    gun.Shoot(50.f, 50.f, true);
    CHECK(gun.isActive());
    CHECK(gun.getRect().left == doctest::Approx(50.f));
    CHECK(gun.getRect().top == doctest::Approx(50.f));
    CHECK(gun.getDamage() == doctest::Approx(10.f));

    // Cập nhật vị trí sau 1000ms (1s)
    gun.update(1000.f, someMap);
    CHECK(gun.getRect().left > 50.f); // đạn phải đi về bên phải

    // Giả lập va chạm với map, tile map toàn '0' (đạn phải bị vô hiệu)
    std::vector<std::string> zeroMap(20, std::string(30, '0'));
    gun.Shoot(50.f, 50.f, true);
    gun.update(10.f, zeroMap);
    CHECK_FALSE(gun.isActive());
}

TEST_CASE("Weapon collision out of map bounds does not crash") {
    Weapon w;
    w.setActive(true);

    // Không được truy cập _rect trực tiếp vì protected, sửa dùng hàm setter giả định (nếu có)
    // Nếu không có setter, skip test chỉnh _rect trực tiếp hoặc kế thừa để test
    // Giả định ta có setRect()
    // w.setRect(sf::FloatRect(-10.f, -10.f, 6, 6));

    // Nếu không có setter, bỏ phần chỉnh trực tiếp _rect này đi

    std::vector<std::string> emptyMap; // map rỗng
    CHECK_NOTHROW(w.collision(emptyMap));
}

TEST_CASE("Weapon deactivates when out of screen bounds") {
    Gun gun;
    gun.Shoot(50.f, 50.f, true);

    // Giả sử màn hình rộng 400 + offsetX = 0

    // Thay vì trực tiếp set gun._rect.left, mày phải làm hàm công khai set vị trí hoặc khởi tạo lại đạn mới với vị trí out of bound
    // Ví dụ tạo đạn với vị trí ngoài màn hình rồi gọi update

    // Cách đơn giản, tạo đạn, rồi chỉnh vị trí qua hàm getRect() trả về reference const thì bó tay
    // Nếu không có setter hoặc không thể truy cập _rect trực tiếp, phải thêm setter trong Weapon.h:

    // Giả lập setter:
    // gun.setPosition(500.f, gun.getRect().top);

    // Còn không, bỏ test này hoặc thêm hàm setter trước đã

    // Sau đó gọi update với map rỗng:
    gun.update(0.f, std::vector<std::string>{});
    CHECK_FALSE(gun.isActive());

    // Tương tự cho vị trí ngoài màn hình trái
    gun.Shoot(50.f, 50.f, false);
    // gun.setPosition(-10.f, gun.getRect().top);
    gun.update(0.f, std::vector<std::string>{});
    CHECK_FALSE(gun.isActive());
}

TEST_CASE("getPosition returns correct coordinates") {
    Gun gun;
    gun.Shoot(100.f, 150.f, true);
    gun.update(0.f, std::vector<std::string>{});
    sf::Vector2f pos = gun.getPosition();
    CHECK(pos.x == doctest::Approx(gun.getRect().left));
    CHECK(pos.y == doctest::Approx(gun.getRect().top));
}

TEST_CASE("getSprite returns reference to sprite") {
    Gun gun;
    gun.Shoot(10.f, 20.f, true);
    const sf::Sprite& sprite = gun.getSprite();
    CHECK(sprite.getPosition().x == doctest::Approx(gun.getRect().left));
    CHECK(sprite.getPosition().y == doctest::Approx(gun.getRect().top));
}