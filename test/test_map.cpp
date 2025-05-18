#include "doctest.h"
#include "Map.h"
#include <fstream>
#include <SFML/Graphics.hpp>

// Tạo file map giả lập để test loadMapFromFile
void createTestMapFile(const std::string& filename) {
    std::ofstream ofs(filename);
    ofs << "11111\n";
    ofs << "1   1\n";
    ofs << "11111\n";
    ofs.close();
}

TEST_CASE("Map getHeight and getWidth return correct constants") {
    Map map;
    CHECK(map.getHeight() == 20);
    CHECK(map.getWidth() == 150);
}

TEST_CASE("Map getMap throws or returns valid for valid level") {
    Map map;
    // load test map first
    const std::string testMapFile = "test_map.txt";
    createTestMapFile(testMapFile);
    map.loadMapFromFile(testMapFile);

    CHECK_NOTHROW(map.getMap(0));
    const auto& m = map.getMap(0);
    CHECK(m.size() == 3);
}

TEST_CASE("Map loadBackground loads image correctly or fails gracefully") {
    Map map;
    // File ảnh không tồn tại, phải return false
    bool result = map.loadBackground("not_exist_image.png");
    CHECK(result == false);

    // Với file ảnh thật thì mới test true, ví dụ:
    // bool result2 = map.loadBackground("valid_background.png");
    // CHECK(result2 == true);
}

TEST_CASE("Global offsetX, offsetY can be set and used") {
    offsetX = 100.f;
    offsetY = 50.f;

    CHECK(offsetX == doctest::Approx(100.f));
    CHECK(offsetY == doctest::Approx(50.f));
}

TEST_CASE("Map loadMapFromFile loads map data correctly") {
    const std::string testMapFile = "test_map.txt";
    createTestMapFile(testMapFile);

    Map map;
    CHECK_NOTHROW(map.loadMapFromFile(testMapFile));
    
    const auto& loadedMap = map.getMap(0);

    CHECK(loadedMap.size() == 3);
    CHECK(loadedMap[0] == "11111");
    CHECK(loadedMap[1] == "1   1");
    CHECK(loadedMap[2] == "11111");

    std::remove(testMapFile.c_str()); // xoá file sau test
}
