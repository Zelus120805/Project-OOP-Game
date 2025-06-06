#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "App.h"
using namespace sf;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    
    App app;
    app.run();
    
    return 0;
}