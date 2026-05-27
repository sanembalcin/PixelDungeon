#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include <cmath>
#include <vector>
#include <fstream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Dungeon - Savasci Surumu");
    window.setFramerateLimit(60);

    DungeonMap map;
    Warrior player;
    Enemy slime(0.f, 0.f);

    std::vector<Item> worldItems;
    worldItems.reserve(20);

    bool slimePotionSpawned = false;
    sf::Vector2f lastSlimePos(0.f, 0.f);
    sf::Clock trapClock;

    bool pWasPressed = false;
    bool lWasPressed = false;

    int itemCounter = 0;
    int potionCounter = 0;
    int swordCounter = 0;
    int armorCounter = 0;

    for (int y = 1; y < MAP_HEIGHT - 1; ++y) {
        for (int x = 1; x < MAP_WIDTH - 1; ++x) {
            if (map.grid[y][x] == 0) {
                if (rand() % 100 < 6 && itemCounter < 8) {
                    float px = x * TILE_SIZE + 8.f;
                    float py = y * TILE_SIZE + 8.f;
                    int r = rand() % 3;

                    if (r == 0 && potionCounter < 4) {
                        worldItems.emplace_back(ItemType::POTION, px, py, "assets/potion.png");
                        potionCounter++;
                        itemCounter++;
                    }
                    else if (r == 1 && swordCounter < 2) {
                        worldItems.emplace_back(ItemType::SWORD, px, py, "assets/sword.png");
                        swordCounter++;
                        itemCounter++;
                    }
                    else if (r == 2 && armorCounter < 2) {
                        worldItems.emplace_back(ItemType::ARMOR, px, py, "assets/armor.png");
                        armorCounter++;
                        itemCounter++;
                    }
                }
                else if (rand() % 100 < 3) {
                    map.grid[y][x] = 3;
                }
            }
        }
    }

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

        bool pPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::P);
        bool lPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::L);

        if (pPressed && !pWasPressed) {
            std::ofstream saveFile("save.txt");
            if (saveFile.is_open()) {
                saveFile << player.getPosition().x << " " << player.getPosition().y << "\n";
                saveFile << player.getHealth() << "\n";
                saveFile.close();
            }
        }

        if (lPressed && !lWasPressed) {
            std::ifstream saveFile("save.txt");
            if (saveFile.is_open()) {
                float px;
                float py;
                int hp;
                saveFile >> px >> py;
                saveFile >> hp;
                player.setPosition(px, py);
                player.setHealth(hp);
                saveFile.close();
            }
        }

        pWasPressed = pPressed;
        lWasPressed = lPressed;

        player.handleInput(map);
        player.update(map);

        int pTileX = static_cast<int>(player.getPosition().x / TILE_SIZE);
        int pTileY = static_cast<int>(player.getPosition().y / TILE_SIZE);

        if (pTileX >= 0 && pTileX < MAP_WIDTH && pTileY >= 0 && pTileY < MAP_HEIGHT) {
            if (map.grid[pTileY][pTileX] == 3) {
                if (trapClock.getElapsedTime().asSeconds() >= 1.0f) {
                    player.takeDamage(5);
                    trapClock.restart();
                }
            }
        }

        if (!slime.isDead()) {
            slime.update(player.getPosition(), map);
            lastSlimePos = slime.getPosition();
        }
        else if (!slimePotionSpawned) {
            worldItems.emplace_back(ItemType::POTION, lastSlimePos.x + 8.f, lastSlimePos.y + 8.f, "assets/potion.png");
            slimePotionSpawned = true;
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

        for (size_t i = 0; i < worldItems.size(); ++i) {
            if (worldItems[i].isActive() && player.getBounds().intersects(worldItems[i].getBounds())) {
                worldItems[i].collect();
                ItemType t = worldItems[i].getType();
                player.addToInventory(t);
            }
        }

        window.clear();

        map.draw(window, player.getPosition());

        for (size_t i = 0; i < worldItems.size(); ++i) {
            sf::FloatRect itemBounds = worldItems[i].getBounds();
            sf::Vector2f itemPos(itemBounds.left, itemBounds.top);
            sf::Vector2f pPos = player.getPosition();

            float distToItem = std::sqrt(std::pow(pPos.x - itemPos.x, 2) + std::pow(pPos.y - itemPos.y, 2));
            float viewRadius = 5.f * TILE_SIZE;

            if (distToItem <= viewRadius) {
                worldItems[i].draw(window, player.getPosition());
            }
        }

        if (!slime.isDead()) {
            sf::Vector2f slimePos = slime.getPosition();
            sf::Vector2f pPos = player.getPosition();

            float distToSlime = std::sqrt(std::pow(pPos.x - slimePos.x, 2) + std::pow(pPos.y - slimePos.y, 2));
            float viewRadius = 5.f * TILE_SIZE;

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