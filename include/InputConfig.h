#ifndef _InputConfig_H_
#define _InputConfig_H_

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

enum class PlayerAction {
    Left,
    Right,
    Up,
    Down,
    Fire,
    Jump
};

class InputConfig {
private:
    static const std::map<std::string, sf::Keyboard::Key> stringToKeyMap;
    static std::map<sf::Keyboard::Key, std::string> keyToStringMap;

    static void initKeyToStringMap();

public:
    static void initialize();
    static std::string keyToString(sf::Keyboard::Key key);
    static sf::Keyboard::Key stringToKey(const std::string& s);

    static bool loadControls(const std::string& filename, std::map<PlayerAction, sf::Keyboard::Key>& p1, std::map<PlayerAction, sf::Keyboard::Key>& p2);
    static void saveControls(const std::string& filename, const std::map<PlayerAction, sf::Keyboard::Key>& p1, const std::map<PlayerAction, sf::Keyboard::Key>& p2);
};

#endif