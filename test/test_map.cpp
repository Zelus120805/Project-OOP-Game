#include "doctest.h"
#include "Map.h"
#include <fstream>
#include <SFML/Graphics.hpp>


TEST_CASE("Map getHeight and getWidth return correct constants") {
    Map map;
    CHECK(map.getHeight() == 24);
    CHECK(map.getWidth() == 150);
}

TEST_CASE("Map getMap throws or returns valid for valid level") {
    Map map;
    // load test map first
    const std::string testMapFile = "Leval/Level_1.txt";
    map.loadMapFromFile(testMapFile);

    CHECK_NOTHROW(map.getMap(0));
    const auto& m = map.getMap(0);
    CHECK(m.size() == 24); // Kiểm tra số dòng
}

TEST_CASE("Map loadBackground loads image correctly or fails gracefully") {
    Map map;
    // File ảnh không tồn tại, phải return false
    bool result = map.loadBackground("not_exist_image.png");
    CHECK(result == false);

    // Với file ảnh thật thì mới test true, ví dụ:
    bool result2 = map.loadBackground("valid_background.png");
    CHECK(result2 == true);
}

TEST_CASE("Global offsetX, offsetY can be set and used") {
    offsetX = 100.f;
    offsetY = 50.f;

    CHECK(offsetX == doctest::Approx(100.f));
    CHECK(offsetY == doctest::Approx(50.f));
}