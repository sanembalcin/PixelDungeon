#include <SFML/Graphics.hpp>
#include "Map.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    
    srand(static_cast<unsigned int>(time(0)));

    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Dungeon - Test");
    window.setFramerateLimit(60);

    DungeonMap dungeonMap;

    std::cout << "Harita yuklendi, oyun basliyor!" << std::endl;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        dungeonMap.draw(window); 
        window.display();
    }

    return 0;
}