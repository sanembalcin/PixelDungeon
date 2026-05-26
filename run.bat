@echo off
cl /std:c++17 main.cpp src/Map.cpp src/Player.cpp src/Enemy.cpp /I"include" /I"C:\SFML_MSVC\include" /link /LIBPATH:"C:\SFML_MSVC\lib" sfml-graphics.lib sfml-window.lib sfml-system.lib opengl32.lib winmm.lib gdi32.lib /OUT:game.exe
if %errorlevel% equ 0 game.exe