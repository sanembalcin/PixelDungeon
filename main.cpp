#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Dungeon - Savasci Surumu");
    window.setFramerateLimit(60);

    DungeonMap map;
    
    Warrior player; 

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // 1. GİRDİLERİ KONTROL ET (Eksik olan ve WASD'yi çalıştıracak satır bu!)
        player.handleInput(map); 

        // 2. MANTIKSAL GÜNCELLEMELERİ YAP
        player.update(map);

        // 3. EKRANA ÇİZME İŞLEMLERİ
        window.clear();
        map.draw(window);
        player.draw(window);
        window.display();
    }
    return 0;
}