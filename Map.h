#ifndef _MAP_H_
#define _MAP_H_

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

inline float offsetX = 0;
inline float offsetY = 0;

class Map {
private:
    const int _height = 17;
    const int _width = 150;
    std::vector<std::vector<std::string>> maps;
public:
    Map() { }

    ~Map() { }

    int getHeight() { return _height; }

    int getWidth() { return _width; }

    const std::vector<std::string>& getMap(int level) const {
        return maps[level];
    }
public:
    // void loadMap() {
    //     for (int i = 0; i < _height; i++) {
    //         for (int j = 0; j < _width; j++) {
    //             char tileChar = currentMap[i][j];
    //             if (tileChar == ' ' || tileChar == '0') continue;

    //             if (tileChar == 'P') tile.setTextureRect(IntRect(143 - 16 * 3, 112, 16, 16));
    //             else if (tileChar == 'k') tile.setTextureRect(IntRect(143, 112, 16, 16));
    //             else if (tileChar == 'c') tile.setTextureRect(IntRect(143 - 16, 112, 16, 16));
    //             else if (tileChar == 't') tile.setTextureRect(IntRect(0, 47, 32, 48));
    //             else if (tileChar == 'g') tile.setTextureRect(IntRect(0, 139, 48, 37));
    //             else if (tileChar == 'G') tile.setTextureRect(IntRect(145, 222, 77, 33));
    //             else if (tileChar == 'd') tile.setTextureRect(IntRect(0, 106, 74, 21));
    //             else if (tileChar == 'w') tile.setTextureRect(IntRect(99, 224, 41, 31));
    //             else if (tileChar == 'r') tile.setTextureRect(IntRect(143 - 32, 112, 16, 16));

    //             tile.setPosition(j * 16 - offsetX, i * 16 - offsetY);
    //             window.draw(tile);
    //         }
    //     }
    // }


    // Hàm đọc map từ file
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
    
        // Kiểm tra độ dài mỗi dòng
        for (int i = 0; i < maps.size(); ++i) {
            for (int j = 0; j < maps[i].size(); ++j) {
                if (maps[i][j].size() != 125) {
                    std::cout << "Dong " << j << " bi loi do dai: " << maps[i][j].size() << "\n";
                }
            }
        }
    
        file.close();
    }  
};

#endif // _MAP_H_