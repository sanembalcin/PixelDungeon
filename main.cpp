#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Dungeon - Savasci Surumu");
    window.setFramerateLimit(60);

    DungeonMap map;
    // Seçim mekanizması yok, doğrudan Savaşçı nesnesi üretiliyor kanka
    Warrior player; 

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.update(map);

        window.clear();
        map.draw(window);
        player.draw(window);
        window.display();
    }

    return 0;
}