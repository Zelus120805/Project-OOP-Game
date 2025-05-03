@echo off
g++ -o main main.cpp Map.cpp Player.cpp Enemy.cpp Bullet.cpp App.cpp -Isrc/include -I/mingw64/include -L/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio