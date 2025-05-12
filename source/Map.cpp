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

bool Map::loadBackground(const std::string& path) {
    if (!_backgroundTexture.loadFromFile(path)) {
        std::cerr << "Failed to load background: " << path << std::endl;
        return false;
    }
    _backgroundSprite.setTexture(_backgroundTexture);
    _backgroundSprite.setPosition(0, 0); // Vẽ từ góc trên bên trái
    return true;
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
    window.draw(_backgroundSprite);         // Vẽ background

    sf::Sprite tile;
    tile.setTexture(tileSet);
    auto& currentMap = getMap(0);

    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            char tileChar = currentMap[i][j];
            if (tileChar == ' ' || tileChar == '0') continue;
            
            if (tileChar == '1') tile.setTextureRect(sf::IntRect(33, 32, 16, 16));          // nền đất 1
            else if (tileChar == '2') tile.setTextureRect(sf::IntRect(49, 32, 16, 16));     // nền đất 2
            else if (tileChar == '3') tile.setTextureRect(sf::IntRect(18, 16, 16, 16));     // nền đất cỏ 1
            else if (tileChar == '4') tile.setTextureRect(sf::IntRect(49, 0, 16, 16));      // nền đất cỏ 2
            else if (tileChar == '5') tile.setTextureRect(sf::IntRect(96, 48, 16, 16));     // block đá nhỏ
            else if (tileChar == '6') tile.setTextureRect(sf::IntRect(112, 32, 32, 32));    // block đá lớn
            else if (tileChar == '7') tile.setTextureRect(sf::IntRect(213, 48, 6, 48));     // thanh gỗ đứng
            else if (tileChar == '8') tile.setTextureRect(sf::IntRect(224, 53, 48, 6));     // thanh gỗ ngang
            else if (tileChar == '9') tile.setTextureRect(sf::IntRect(209, 33, 15, 15));    // cỏ hình cầu
            else continue;

            tile.setPosition(j * 16 - offsetX, i * 16 - offsetY);
            window.draw(tile);
        }
    }
}