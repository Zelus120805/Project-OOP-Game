all:
	g++ -o game \
	    source/main.cpp source/App.cpp source/Player.cpp source/Enemy.cpp source/Map.cpp source/Bullet.cpp \
	    -Iinclude \
	    -L/mingw64/lib \
	    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
# Unit test với doctest
test:
	g++ -o test_exec test/test_player.cpp source/Player.cpp source/Bullet.cpp source/Enemy.cpp source/Map.cpp \
	    -Iinclude -Itest -Itest/doctest-master/doctest -I/mingw64/include \
	    -L/mingw64/lib \
	    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./test_exec