#include "Error.h"

void ErrorMessage::show(const std::string& msg) {
    std::cout << "Error: " << msg << std::endl;
}

bool ResourceLoader::loadFont(sf::Font& font, const std::string& filepath) {
    if (!font.loadFromFile(filepath)) {
        std::cout << "Error loading font from: " << filepath << std::endl;
        return false;
    }

    return true;
}

bool ResourceLoader::loadTexture(sf::Texture& texture, const std::string& filepath) {
    if (!texture.loadFromFile(filepath)) {
        std::cout << "Error loading texture from: " << filepath << std::endl;
        return false;
    }
    
    return true;
}

bool ResourceLoader::loadSound(sf::Music& music, const std::string& filepath) {
    if (!music.openFromFile(filepath)) {
        std::cout << "Error loading sound from: " << filepath << std::endl;
        return false;
    }

    return true;
}