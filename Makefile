all:
	g++ -o game main.cpp App.h App.cpp Player.h Player.cpp Enemy.h Enemy.cpp Map.h Map.cpp Bullet.h Bullet.cpp\
	    -Isrc/include \
	    -I/mingw64/include -L/mingw64/lib \
	    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio