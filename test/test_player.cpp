#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Player.h"

TEST_CASE("Player basic movement state") {
    Player player;

    // Kiểm tra mặc định dy = 0
    CHECK(player.getDY() == doctest::Approx(0.0f));

    // Gán thử dy và kiểm tra lại
    player.setDY(5.5f);
    CHECK(player.getDY() == doctest::Approx(5.5f));
}

TEST_CASE("Player isHit flag and flashCount") {
    Player player;

    player.setIsHit(true);
    CHECK(player.getIsHit() == true);

    player.setFlashCount(3);
    // Không có getter flashCount, nên tạm không test được giá trị này.
}
