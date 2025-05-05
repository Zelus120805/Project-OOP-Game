all:
	g++ -o game main.cpp App.cpp Player.cpp Enemy.cpp Map.cpp Bullet.cpp\
	    -Isrc/include \
	    -I/mingw64/include -L/mingw64/lib \
	    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio