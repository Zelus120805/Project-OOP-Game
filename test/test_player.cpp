#include "doctest.h"
#include "Player.h"
#include <vector>
#include <string>

// Mock map đơn giản, toàn ' ' (không cản)
std::vector<std::string> emptyMap = {
    "                                        ",
    "                                        ",
    "                                        ",
    "                                        ",
    "                                        ",
    "                                        ",
    "                                        ",
    "                                        ",
    "                                        ",
    "                                        "
};

TEST_CASE("Player Contra init và di chuyển basic") {
    Contra player;
    player.setPlayer(50.f, 50.f);

    // Check khởi tạo HP
    CHECK(player.getHP() == doctest::Approx(100.f));

    // Ban đầu chưa đi, dx = 0
    CHECK(player.getDY() == 0);

    // Giả lập bấm phím phải (right)
    player.controlPlayer(sf::Keyboard::Right, sf::Keyboard::Right, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Space, sf::Keyboard::F);
    // Nhưng do isKeyPressed chỉ check thật, ta test kiểu update để giả lập:
    player.setDY(0);
    player.update(10.f, emptyMap, *(new sf::RenderWindow()));

    // Tạm thời chưa di chuyển thật vì isKeyPressed không giả lập được
    // Nhưng check vẫn giữ HP đầy đủ
    CHECK(player.getHP() == doctest::Approx(100.f));
}

TEST_CASE("Player Contra bị trúng đạn giảm HP") {
    Contra player;
    player.setPlayer(50.f, 50.f);

    // Ban đầu full HP
    CHECK(player.getHP() == doctest::Approx(100.f));

    // Set trạng thái bị trúng đạn
    player.setIsHit(true);

    // Gọi isAttacked() để xử lý trừ máu
    player.testisAttacked();

    // HP giảm 10
    CHECK(player.getHP() == doctest::Approx(90.f));

    // Giữ trạng thái isHit false sau 6 lần nhấp nháy (giả lập flashCount)
    player.setFlashCount(6);
    player.testisAttacked();

    CHECK(player.getIsHit() == false);
}

TEST_CASE("Player Contra bắn đạn tạo bullet active") {
    Contra player;
    player.setPlayer(50.f, 50.f);

    player.attack();

    auto& bullets = player.getBullets();
    CHECK(bullets.size() > 0);

    // Đạn phải đang active
    CHECK(bullets.back().isActive());
}