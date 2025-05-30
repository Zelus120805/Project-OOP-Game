#include "InputConfig.h"

std::map<sf::Keyboard::Key, std::string> InputConfig::keyToStringMap;

const std::map<std::string, sf::Keyboard::Key> InputConfig::stringToKeyMap = {
    {"A", sf::Keyboard::A}, {"B", sf::Keyboard::B}, {"C", sf::Keyboard::C}, {"D", sf::Keyboard::D}, {"E", sf::Keyboard::E},
    {"F", sf::Keyboard::F}, {"G", sf::Keyboard::G}, {"H", sf::Keyboard::H}, {"I", sf::Keyboard::I}, {"J", sf::Keyboard::J},
    {"K", sf::Keyboard::K}, {"L", sf::Keyboard::L}, {"M", sf::Keyboard::M}, {"N", sf::Keyboard::N}, {"O", sf::Keyboard::O},
    {"P", sf::Keyboard::P}, {"Q", sf::Keyboard::Q}, {"R", sf::Keyboard::R}, {"S", sf::Keyboard::S}, {"T", sf::Keyboard::T},
    {"U", sf::Keyboard::U}, {"V", sf::Keyboard::V}, {"W", sf::Keyboard::W}, {"X", sf::Keyboard::X}, {"Y", sf::Keyboard::Y},
    {"Z", sf::Keyboard::Z}, {"Space", sf::Keyboard::Space}, {"Enter", sf::Keyboard::Enter}, {"LControl", sf::Keyboard::LControl},
    {"RControl", sf::Keyboard::RControl}, {"LShift", sf::Keyboard::LShift}, {"RShift", sf::Keyboard::RShift},
    {"Up", sf::Keyboard::Up}, {"Down", sf::Keyboard::Down}, {"Left", sf::Keyboard::Left}, {"Right", sf::Keyboard::Right},
    {"Num0", sf::Keyboard::Num0}, {"Num1", sf::Keyboard::Num1}, {"Num2", sf::Keyboard::Num2}, {"Num3", sf::Keyboard::Num3},
    {"Num4", sf::Keyboard::Num4}, {"Num5", sf::Keyboard::Num5}, {"Num6", sf::Keyboard::Num6}, {"Num7", sf::Keyboard::Num7},
    {"Num8", sf::Keyboard::Num8}, {"Num9", sf::Keyboard::Num9},
    {"Numpad0", sf::Keyboard::Numpad0}, {"Numpad1", sf::Keyboard::Numpad1}, {"Numpad2", sf::Keyboard::Numpad2},
    {"Numpad3", sf::Keyboard::Numpad3}, {"Numpad4", sf::Keyboard::Numpad4}, {"Numpad5", sf::Keyboard::Numpad5},
    {"Numpad6", sf::Keyboard::Numpad6}, {"Numpad7", sf::Keyboard::Numpad7}, {"Numpad8", sf::Keyboard::Numpad8},
    {"Numpad9", sf::Keyboard::Numpad9}
};

void InputConfig::initKeyToStringMap() {
    for (auto& [str, key] : stringToKeyMap) {
        keyToStringMap[key] = str;
    }
}

void InputConfig::initialize() {
    initKeyToStringMap();
}

std::string InputConfig::keyToString(sf::Keyboard::Key key) {
    auto it = keyToStringMap.find(key);
    if (it != keyToStringMap.end()) 
        return it->second;

    return "Unknown";
}

sf::Keyboard::Key InputConfig::stringToKey(const std::string& s) {
    auto it = stringToKeyMap.find(s);
    if (it != stringToKeyMap.end()) 
        return it->second;

    return sf::Keyboard::Unknown;
}

bool InputConfig::loadControls(const std::string& filename, std::map<PlayerAction, sf::Keyboard::Key>& p1, std::map<PlayerAction, sf::Keyboard::Key>& p2) {
    std::ifstream file(filename);
    if (!file.is_open())
        return false;

    std::string line;
    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string label;
        iss >> label;

        std::vector<std::string> keys;
        std::string key;
        while (iss >> key) keys.push_back(key);
        if (keys.size() != 6) continue;

        std::map<PlayerAction, sf::Keyboard::Key> temp = {
            {PlayerAction::Left,  stringToKey(keys[0])},
            {PlayerAction::Right, stringToKey(keys[1])},
            {PlayerAction::Down,  stringToKey(keys[2])},
            {PlayerAction::Up,    stringToKey(keys[3])},
            {PlayerAction::Fire,  stringToKey(keys[4])},
            {PlayerAction::Jump,  stringToKey(keys[5])}
        };

        if (label == "Player1:") p1 = temp;
        else if (label == "Player2:") p2 = temp;
    }

    file.close();
    return true;
}

void InputConfig::saveControls(const std::string& filename, const std::map<PlayerAction, sf::Keyboard::Key>& p1, const std::map<PlayerAction, sf::Keyboard::Key>& p2) {
    std::ofstream file(filename);
    if (!file.is_open()) 
        return;

    file << "Player1:";
    for (int i = 0; i < 6; ++i)
        file << " " << keyToString(p1.at(static_cast<PlayerAction>(i)));
    file << "\n";

    file << "Player2:";
    for (int i = 0; i < 6; ++i)
        file << " " << keyToString(p2.at(static_cast<PlayerAction>(i)));

    file.close();
}