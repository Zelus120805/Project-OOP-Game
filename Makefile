all:
	g++ -o game main.cpp App.h Player.h Enemy.h Map.h \
	    -Isrc/include \
	    -I/mingw64/include -L/mingw64/lib \
	    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio