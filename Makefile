all:
	g++ -o game main.cpp App.cpp Player.cpp Enemy.cpp Map.cpp Bullet.cpp\
	    -Isrc/include \
	    -I/mingw64/include -L/mingw64/lib \
	    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
		
test: test/test_player.cpp Player.cpp Bullet.cpp
	g++ -o test_exec test/test_player.cpp Player.cpp Enemy.cpp Map.cpp Bullet.cpp \
	    -I. -Itest -Itest/doctest-master/doctest -I/mingw64/include \
	    -L/mingw64/lib \
	    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./test_exec