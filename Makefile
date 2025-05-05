all:
	g++ -o game \
	    source/main.cpp source/App.cpp source/Player.cpp source/Enemy.cpp source/Map.cpp source/Bullet.cpp \
	    -Iinclude \
	    -L/mingw64/lib \
	    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
