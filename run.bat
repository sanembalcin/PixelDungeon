@echo off
cl /EHsc /Iinclude /IC:\SFML_MSVC\include main.cpp src/Map.cpp src/Player.cpp src/Enemy.cpp src/Potion.cpp /link /LIBPATH:C:\SFML_MSVC\lib sfml-graphics.lib sfml-window.lib sfml-system.lib opengl32.lib winmm.lib gdi32.lib /OUT:game.exe
game.exe