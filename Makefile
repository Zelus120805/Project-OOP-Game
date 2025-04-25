all: compile link

compile:
	g++ -Isrc/include -c App.h Player.h Enemy.h Map.h main.cpp

link:
	g++ main.cpp -o game -I/mingw64/include -L/mingw64/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio