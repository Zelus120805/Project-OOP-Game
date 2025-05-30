#ifndef _ERROR_H_
#define _ERROR_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdexcept>
#include <string>

class ErrorMessage {
public:
    void show(const std::string& msg);
};

class ResourceLoader {
private:
    ErrorMessage errorHandler;
public:
    static bool loadFont(sf::Font& font, const std::string& filepath);
    static bool loadTexture(sf::Texture& texture, const std::string& filepath);
    static bool loadSound(sf::Music& music, const std::string& filepath); 
};

#endif // _ERROR_H_