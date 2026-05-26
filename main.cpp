#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Dungeon - Savasci Surumu");
    window.setFramerateLimit(60);

    DungeonMap map;
    Warrior player;
    Enemy slime(160.f, 160.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.handleInput(map); 
        player.update(map);
        slime.update(player.getPosition(), map);

        window.clear();
        
        map.draw(window);
        slime.draw(window);
        player.draw(window);
        
        window.display();
    }
    return 0;
}