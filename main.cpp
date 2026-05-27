#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Potion.h"
#include <cmath>
#include <vector>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Dungeon - Savasci Surumu");
    window.setFramerateLimit(60);

    DungeonMap map;
    Warrior player;
    Enemy slime(0.f, 0.f);
    
    std::vector<Potion> potions;
    bool potionSpawned = false;
    sf::Vector2f lastSlimePos(0.f, 0.f);

    sf::Vector2f safePlayerPos(100.f, 100.f);
    sf::Vector2f safeSlimePos(400.f, 300.f);
    bool playerPlaced = false;

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (map.grid[y][x] == 0 && !playerPlaced) {
                safePlayerPos = sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
                playerPlaced = true;
                break;
            }
        }
        if (playerPlaced) break;
    }

    bool slimePlaced = false;
    for (int y = MAP_HEIGHT - 1; y >= 0; --y) {
        for (int x = MAP_WIDTH - 1; x >= 0; --x) {
            if (map.grid[y][x] == 0 && !slimePlaced) {
                safeSlimePos = sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
                slimePlaced = true;
                break;
            }
        }
        if (slimePlaced) break;
    }

    player.setPosition(safePlayerPos.x, safePlayerPos.y);
    slime.setPosition(safeSlimePos.x, safeSlimePos.y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        player.update(map);
        
        if (!slime.isDead()) {
            slime.update(player.getPosition(), map);
            lastSlimePos = slime.getPosition();
        } else if (!potionSpawned) {
            potions.emplace_back(lastSlimePos.x + 45.f, lastSlimePos.y + 45.f);
            potionSpawned = true;
        }

        if (player.getIsAttacking() && player.getBounds().intersects(slime.getBounds()) && !slime.isDead()) {
            slime.takeDamage(25);
            sf::Vector2f slimePush = slime.getPosition() - player.getPosition();
            float sLength = std::sqrt(slimePush.x * slimePush.x + slimePush.y * slimePush.y);
            if (sLength != 0) {
                slimePush /= sLength;
                slime.setPosition(slime.getPosition().x + slimePush.x * 30.f, slime.getPosition().y + slimePush.y * 30.f);
            }
        }

        if (!slime.isDead() && player.getBounds().intersects(slime.getBounds())) {
            player.takeDamage(15);
        }

        if (slime.isDead()) {
            slime.setPosition(-9999.f, -9999.f);
        }

        for (size_t i = 0; i < potions.size(); ++i) {
            if (potions[i].isActive() && player.getBounds().intersects(potions[i].getBounds())) {
                potions[i].collect();
                player.heal(30);
            }
        }

        window.clear();
        
        map.draw(window, player.getPosition());
        
        for (size_t i = 0; i < potions.size(); ++i) {
            potions[i].draw(window, player.getPosition());
        }

        if (!slime.isDead()) {
            sf::Vector2f slimePos = slime.getPosition();
            sf::Vector2f pPos = player.getPosition();
            float distToSlime = std::sqrt(std::pow(pPos.x - slimePos.x, 2) + std::pow(pPos.y - slimePos.y, 2));
            float viewRadius = 5.f * 32.f;

            if (distToSlime <= viewRadius) {
                slime.draw(window);
            }
        }
        
        player.draw(window);
        player.drawHealthBar(window);
        
        window.display();
    }

    return 0;
}