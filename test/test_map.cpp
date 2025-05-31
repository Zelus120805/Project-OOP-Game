#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Map.h"
#include "MyException.h"
#include <fstream>

TEST_CASE("Map loads a valid map file") {
    // Tạo file tạm cho test
    std::ofstream testFile("Level/test_level.txt");
    testFile << "1111\n"
             << "1  1\n"
             << "1111\n\n"
             << "2 2 2 2\n"
             << "2    2\n";
    testFile.close();

    Map map;
    map.loadMapFromFile("Level/test_level.txt");

    CHECK(map.getMap(0).size() == 24);  // Map 1 có 24 dòng
    CHECK(map.getMap(1).size() == 3);  // Map 2 có 3 dòng
    CHECK(map.getMap(2).size() == 2);   // Map 3 có 2 dòng
}

TEST_CASE("Map getHeight and getWidth getters") {
    Map map;
    CHECK(map.getHeight() == 24);
    CHECK(map.getWidth() == 150);
}
