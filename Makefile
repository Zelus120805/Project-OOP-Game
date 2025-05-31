.PHONY: all test
all:
	g++ -o game \
	    source/main.cpp source/App.cpp source/Player.cpp source/Enemy.cpp source/Map.cpp source/Weapon.cpp source/MyException.cpp source/Random.cpp source/InputConfig.cpp source/PlayerFactory.cpp \
	    -Iinclude \
	    -L/mingw64/lib \
	    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
# Unit test với doctest
testplayer:
	g++ -o test_player_exec \
    test/test_player.cpp source/Player.cpp source/Weapon.cpp source/MyException.cpp source/Map.cpp \
    -Iinclude -Itest -Itest/doctest-master/doctest -I/mingw64/include \
    -L/mingw64/lib \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./test_player_exec
testweapon:
	g++ -o test_weapon_exec \
	test/test_weapon.cpp source/Weapon.cpp source/Player.cpp source/MyException.cpp source/Map.cpp \
	-Iinclude -Itest -Itest/doctest-master/doctest -I/mingw64/include \
	-L/mingw64/lib \
	-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./test_weapon_exec
testmap:
	g++ -o test_map_exec \
	test/test_map.cpp source/Map.cpp source/Player.cpp source/Weapon.cpp source/Enemy.cpp source/MyException.cpp \
	-Iinclude -Itest -Itest/doctest-master/doctest -I/mingw64/include \
	-L/mingw64/lib \
	-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./test_map_exec
testmyexception:
	g++ -o test_myexception_exec \
	test/test_myexception.cpp source/MyException.cpp source/Map.cpp source/Player.cpp source/Weapon.cpp \
	-Iinclude -Itest -Itest/doctest-master/doctest -I/mingw64/include \
	-L/mingw64/lib \
	-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
	./test_myexception_exec
