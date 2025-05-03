#ifndef _MAP_H_
#define _MAP_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
using namespace sf;

inline float offsetX = 0;
inline float offsetY = 0;

class Map {
private:
    const int _height = 20;
    const int _width = 150;
    std::vector<std::vector<std::string>> maps;
public:
    Map() {
        if (!maps.empty()) {
            maps.clear();
        }
        try {
            this->loadMapFromFile("Level/Level_1.txt"); // Đọc map từ file
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    ~Map() { }

    int getHeight() { return _height; }

    int getWidth() { return _width; }

    const std::vector<std::string>& getMap(int level) const {
        return maps[level];
    }
public:
void loadMapFromFile(const std::string& filePath) {
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

void render(RenderWindow& window, const Texture& tileSet) {
    Sprite tile;
    tile.setTexture(tileSet);
    auto& currentMap = getMap(0);

    for (int i = 0; i < _height; i++) {
        for (int j = 0; j < _width; j++) {
            char tileChar = currentMap[i][j];
            if (tileChar == ' ' || tileChar == '0') continue;

            if (tileChar == 'P') tile.setTextureRect(IntRect(143 - 16 * 3, 112, 16, 16));   //Gạch nền thường (brick)
            else if (tileChar == 'k') tile.setTextureRect(IntRect(143, 112, 16, 16));       //gạch tường thành
            else if (tileChar == 'c') tile.setTextureRect(IntRect(143 - 16, 112, 16, 16));  //"?"
            else if (tileChar == 't') tile.setTextureRect(IntRect(0, 47, 32, 48));          //Ống nước (ống đứng, 2 block)
            else if (tileChar == 'g') tile.setTextureRect(IntRect(0, 139, 48, 37));         //Bụi cây nhỏ / thấp
            else if (tileChar == 'G') tile.setTextureRect(IntRect(145, 222, 77, 33));       //Bụi cây lớn / rậm
            else if (tileChar == 'd') tile.setTextureRect(IntRect(0, 106, 74, 21));         //bụi cây
            else if (tileChar == 'w') tile.setTextureRect(IntRect(99, 224, 41, 31));        //Mây
            else if (tileChar == 'r') tile.setTextureRect(IntRect(143 - 32, 112, 16, 16));  //gạch nâu
            else continue; // Nếu không phải các loại tile đã định nghĩa, bỏ qua
            
            tile.setPosition(j * 16 - offsetX, i * 16 - offsetY);
            window.draw(tile);
        }
    }
}
};

#endif // _MAP_H_