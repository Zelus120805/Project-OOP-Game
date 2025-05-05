#include "Map.h"
#include <fstream>
#include <iostream>

float offsetX = 0;
float offsetY = 0;

Map::Map() {
    if (!maps.empty()) {
        maps.clear();
    }
    try {
        this->loadMapFromFile("Level/Level_1.txt");
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

Map::~Map() { }

int Map::getHeight() { return _height; }

int Map::getWidth() { return _width; }

const std::vector<std::string>& Map::getMap(int level) const {
    return maps[level];
}

void Map::loadMapFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open map file: " + filePath);
    }

    std::vector<std::string> map;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            if (!map.empty()) {
                maps.push_back(map);
                map.clear();
            }
        } else {
            map.push_back(line);
        }
    }
    if (!map.empty()) {
        maps.push_back(map);
    }

    file.close();
}

void Map::render(sf::RenderWindow& window, const sf::Texture& tileSet) {
    sf::Sprite tile;
    tile.setTexture(tileSet);
    auto& currentMap = getMap(0);

    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            char tileChar = currentMap[i][j];
            if (tileChar == ' ' || tileChar == '0') continue;

            if (tileChar == 'P') tile.setTextureRect(sf::IntRect(143 - 16 * 3, 112, 16, 16));
            else if (tileChar == 'k') tile.setTextureRect(sf::IntRect(143, 112, 16, 16));
            else if (tileChar == 'c') tile.setTextureRect(sf::IntRect(143 - 16, 112, 16, 16));
            else if (tileChar == 't') tile.setTextureRect(sf::IntRect(0, 47, 32, 48));
            else if (tileChar == 'g') tile.setTextureRect(sf::IntRect(0, 139, 48, 37));
            else if (tileChar == 'G') tile.setTextureRect(sf::IntRect(145, 222, 77, 33));
            else if (tileChar == 'd') tile.setTextureRect(sf::IntRect(0, 106, 74, 21));
            else if (tileChar == 'w') tile.setTextureRect(sf::IntRect(99, 224, 41, 31));
            else if (tileChar == 'r') tile.setTextureRect(sf::IntRect(143 - 32, 112, 16, 16));
            else continue;

            tile.setPosition(j * 16 - offsetX, i * 16 - offsetY);
            window.draw(tile);
        }
    }
}