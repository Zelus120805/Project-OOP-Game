#ifndef _MAP_H_
#define _MAP_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

extern float offsetX;
extern float offsetY;

class Map {
private:
    const int _height = 22;
    const int _width = 150;
    std::vector<std::vector<std::string>> maps;
    sf::Texture _backgroundTexture;
    sf::Sprite _backgroundSprite;
public:
    Map();
    ~Map();

    int getHeight();
    int getWidth();
    const std::vector<std::string>& getMap(int level) const;
    bool loadBackground(const std::string& path);

    void loadMapFromFile(const std::string& filePath);
    void render(sf::RenderWindow& window, const sf::Texture& tileSet);
};

#endif // _MAP_H_