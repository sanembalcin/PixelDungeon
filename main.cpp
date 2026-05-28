#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include "Slime.h"
#include "Skeleton.h"
#include "MageEnemy.h"
#include "Item.h"
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <memory>

enum class GameState {
    MENU,
    CHARACTER_SELECT,
    PLAYING
};

sf::Vector2f findFirstFloorTile(DungeonMap& map) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (map.grid[y][x] == 0) {
                return sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
            }
        }
    }
    return sf::Vector2f(100.f, 100.f);
}

sf::Vector2f findLastFloorTile(DungeonMap& map) {
    for (int y = MAP_HEIGHT - 1; y >= 0; --y) {
        for (int x = MAP_WIDTH - 1; x >= 0; --x) {
            if (map.grid[y][x] == 0) {
                return sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
            }
        }
    }
    return sf::Vector2f(400.f, 300.f);
}

sf::Vector2f findMiddleFloorTile(DungeonMap& map) {
    for (int y = MAP_HEIGHT / 2; y < MAP_HEIGHT; ++y) {
        for (int x = MAP_WIDTH / 2; x < MAP_WIDTH; ++x) {
            if (map.grid[y][x] == 0) {
                return sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
            }
        }
    }
    return findLastFloorTile(map);
}

void spawnItemsAndTraps(DungeonMap& map, std::vector<Item>& worldItems) {
    worldItems.clear();

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
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Dungeon - Savasci Surumu");
    window.setFramerateLimit(60);

    DungeonMap map;

    int selectedCharacter = 0;
    GameState gameState = GameState::MENU;

    std::unique_ptr<Player> player;

    if (selectedCharacter == 0) {
        player = std::make_unique<Warrior>();
    }
    else {
        player = std::make_unique<Rogue>();
    }

    Slime slime(0.f, 0.f);
    Skeleton skeleton(-9999.f, -9999.f);
    MageEnemy mage(-9999.f, -9999.f);

    std::vector<Item> worldItems;
    worldItems.reserve(20);

    int currentFloor = 1;
    bool slimePotionSpawned = false;
    sf::Vector2f lastSlimePos(0.f, 0.f);
    sf::Clock trapClock;
    sf::Clock playerAttackClock;

    bool pWasPressed = false;
    bool lWasPressed = false;

    spawnItemsAndTraps(map, worldItems);

    sf::Vector2f playerStart = findFirstFloorTile(map);
    sf::Vector2f slimeStart = findLastFloorTile(map);

    player->setPosition(playerStart.x, playerStart.y);
    slime.setPosition(slimeStart.x, slimeStart.y);

    sf::Font font;
    font.loadFromFile("assets/dungeon_font.otf");

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (gameState == GameState::MENU) {

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                gameState = GameState::CHARACTER_SELECT;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

            window.clear(sf::Color::Black);

            sf::Text title;
            title.setFont(font);
            title.setString("PIXEL DUNGEON");
            title.setCharacterSize(42);
            title.setFillColor(sf::Color(180, 180, 255));
            title.setPosition(180.f, 150.f);

            sf::Text startText;
            startText.setFont(font);
            startText.setString("ENTER - START");
            startText.setCharacterSize(22);
            startText.setFillColor(sf::Color::White);
            startText.setPosition(250.f, 300.f);

            sf::Text exitText;
            exitText.setFont(font);
            exitText.setString("ESC - QUIT");
            exitText.setCharacterSize(22);
            exitText.setFillColor(sf::Color::White);
            exitText.setPosition(270.f, 350.f);

            window.draw(title);
            window.draw(startText);
            window.draw(exitText);

             window.display();

            continue;
}

        bool pPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::P);
        bool lPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::L);

        if (pPressed && !pWasPressed) {
            std::ofstream saveFile("save.txt");

            if (saveFile.is_open()) {
                saveFile << player->getPosition().x << " " << player->getPosition().y << "\n";
                saveFile << player->getHealth() << "\n";
                saveFile << currentFloor << "\n";
                saveFile.close();
            }
        }

        if (lPressed && !lWasPressed) {
            std::ifstream saveFile("save.txt");

            if (saveFile.is_open()) {
                float px;
                float py;
                int hp;
                int floor;

                saveFile >> px >> py;
                saveFile >> hp;
                saveFile >> floor;

                player->setPosition(px, py);
                player->setHealth(hp);
                currentFloor = floor;

                if (currentFloor >= 2) {
                    sf::Vector2f skelPos = findMiddleFloorTile(map);
                    skeleton = Skeleton(skelPos.x, skelPos.y);
                }
                else {
                    skeleton.setPosition(-9999.f, -9999.f);
                }

                if (currentFloor >= 3) {
                    sf::Vector2f magePos = findMiddleFloorTile(map);
                    mage = MageEnemy(magePos.x, magePos.y);
                }
                else {
                    mage = MageEnemy(-9999.f, -9999.f);
                }

                saveFile.close();
            }
        }

        pWasPressed = pPressed;
        lWasPressed = lPressed;

        player->handleInput(map);
        player->update(map);

        int pTileX = static_cast<int>((player->getPosition().x + 16.f) / TILE_SIZE);
        int pTileY = static_cast<int>((player->getPosition().y + 16.f) / TILE_SIZE);

        if (pTileX >= 0 && pTileX < MAP_WIDTH && pTileY >= 0 && pTileY < MAP_HEIGHT) {
            if (map.grid[pTileY][pTileX] == 3) {
                if (trapClock.getElapsedTime().asSeconds() >= 1.0f) {
                    player->takeDamage(5);
                    trapClock.restart();
                }
            }

            if (map.grid[pTileY][pTileX] == 4) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    currentFloor++;

                    map.generateNewMap();
                    spawnItemsAndTraps(map, worldItems);

                    sf::Vector2f newPlayerPos = findFirstFloorTile(map);
                    sf::Vector2f newSlimePos = findLastFloorTile(map);
                    sf::Vector2f newSkeletonPos = findMiddleFloorTile(map);
                    sf::Vector2f newMagePos = findMiddleFloorTile(map);

                    player->setPosition(newPlayerPos.x, newPlayerPos.y);
                    slime.setPosition(newSlimePos.x, newSlimePos.y);

                    if (currentFloor >= 2) {
                        skeleton = Skeleton(newSkeletonPos.x, newSkeletonPos.y);
                    }
                    else {
                        skeleton.setPosition(-9999.f, -9999.f);
                    }

                    if (currentFloor >= 3) {
                        mage = MageEnemy(newMagePos.x, newMagePos.y);
                    }
                    else {
                        mage = MageEnemy(-9999.f, -9999.f);
                    }

                    slimePotionSpawned = false;
                }
            }
        }

        if (!slime.isDead()) {
            slime.update(player->getPosition(), map);
            lastSlimePos = slime.getPosition();
        }
        else if (!slimePotionSpawned) {
            worldItems.emplace_back(ItemType::POTION, lastSlimePos.x + 8.f, lastSlimePos.y + 8.f, "assets/potion.png");
            slimePotionSpawned = true;
        }

        if (currentFloor >= 2 && !skeleton.isDead()) {
            skeleton.update(player->getPosition(), map);

            if (skeleton.getBounds().intersects(player->getBounds())) {
                if (skeleton.tryAttack()) {
                    player->takeDamage(skeleton.getDamage());
                }
            }
        }

        if (currentFloor >= 3 && !mage.isDead()) {
            mage.update(player->getPosition(), map);

            std::vector<Projectile>& projectiles = mage.getProjectiles();

            for (size_t i = 0; i < projectiles.size(); ++i) {
                if (projectiles[i].isActive() && projectiles[i].getBounds().intersects(player->getBounds())) {
                    player->takeDamage(projectiles[i].getDamage());
                    projectiles[i].deactivate();
                }
            }
        }

        if (player->getIsAttacking() && player->getBounds().intersects(slime.getBounds()) && !slime.isDead()) {
            if (playerAttackClock.getElapsedTime().asSeconds() >= 0.4f) {
                slime.takeDamage(25);

                sf::Vector2f slimePush = slime.getPosition() - player->getPosition();

                float sLength = std::sqrt(
                    slimePush.x * slimePush.x +
                    slimePush.y * slimePush.y
                );

                if (sLength != 0) {
                    slimePush /= sLength;

                    slime.setPosition(
                        slime.getPosition().x + slimePush.x * 30.f,
                        slime.getPosition().y + slimePush.y * 30.f
                    );
                }

                playerAttackClock.restart();
            }
        }

        if (currentFloor >= 2 &&
            player->getIsAttacking() &&
            player->getBounds().intersects(skeleton.getBounds()) &&
            !skeleton.isDead()) {

            if (playerAttackClock.getElapsedTime().asSeconds() >= 0.4f) {
                skeleton.takeDamage(25);

                if (!skeleton.isDead()) {
                    sf::Vector2f pushDir = skeleton.getPosition() - player->getPosition();

                    float length = std::sqrt(
                        pushDir.x * pushDir.x +
                        pushDir.y * pushDir.y
                    );

                    if (length != 0) {
                        pushDir /= length;

                        float newSkelX = skeleton.getPosition().x + pushDir.x * 35.f;
                        float newSkelY = skeleton.getPosition().y + pushDir.y * 35.f;

                        int skelTileX = static_cast<int>((newSkelX + 16.f) / TILE_SIZE);
                        int skelTileY = static_cast<int>((newSkelY + 16.f) / TILE_SIZE);

                        if (skelTileX >= 0 && skelTileX < MAP_WIDTH &&
                            skelTileY >= 0 && skelTileY < MAP_HEIGHT &&
                            map.grid[skelTileY][skelTileX] != 1) {

                            skeleton.setPosition(newSkelX, newSkelY);
                        }
                    }
                }

                playerAttackClock.restart();
            }
        }

        if (currentFloor >= 3 &&
            player->getIsAttacking() &&
            player->getBounds().intersects(mage.getBounds()) &&
            !mage.isDead()) {

            if (playerAttackClock.getElapsedTime().asSeconds() >= 0.4f) {
                mage.takeDamage(25);
                playerAttackClock.restart();
            }
        }

        if (!slime.isDead() && player->getBounds().intersects(slime.getBounds())) {
            player->takeDamage(15);
        }

        if (slime.isDead()) {
            slime.setPosition(-9999.f, -9999.f);
        }

        for (size_t i = 0; i < worldItems.size(); ++i) {
            if (worldItems[i].isActive() && player->getBounds().intersects(worldItems[i].getBounds())) {
                worldItems[i].collect();
                ItemType t = worldItems[i].getType();
                player->addToInventory(t);
            }
        }

        window.clear();

        map.draw(window, player->getPosition());

        for (size_t i = 0; i < worldItems.size(); ++i) {
            sf::FloatRect itemBounds = worldItems[i].getBounds();
            sf::Vector2f itemPos(itemBounds.left, itemBounds.top);
            sf::Vector2f pPos = player->getPosition();

            float distToItem = std::sqrt(
                std::pow(pPos.x - itemPos.x, 2) +
                std::pow(pPos.y - itemPos.y, 2)
            );

            float viewRadius = 5.f * TILE_SIZE;

            if (distToItem <= viewRadius) {
                worldItems[i].draw(window, player->getPosition());
            }
        }

        if (!slime.isDead()) {
            sf::Vector2f slimePos = slime.getPosition();
            sf::Vector2f pPos = player->getPosition();

            float distToSlime = std::sqrt(
                std::pow(pPos.x - slimePos.x, 2) +
                std::pow(pPos.y - slimePos.y, 2)
            );

            float viewRadius = 5.f * TILE_SIZE;

            if (distToSlime <= viewRadius) {
                slime.draw(window);
            }
        }

        if (currentFloor >= 2) {
            sf::Vector2f skeletonPos = skeleton.getPosition();
            sf::Vector2f pPos = player->getPosition();

            float distToSkeleton = std::sqrt(
                std::pow(pPos.x - skeletonPos.x, 2) +
                std::pow(pPos.y - skeletonPos.y, 2)
            );

            float viewRadius = 5.f * TILE_SIZE;

            if (distToSkeleton <= viewRadius) {
                skeleton.draw(window);
            }
        }

        if (currentFloor >= 3) {
            sf::Vector2f magePos = mage.getPosition();
            sf::Vector2f pPos = player->getPosition();

            float distToMage = std::sqrt(
                std::pow(pPos.x - magePos.x, 2) +
                std::pow(pPos.y - magePos.y, 2)
            );

            float viewRadius = 5.f * TILE_SIZE;

            if (distToMage <= viewRadius) {
                mage.draw(window);
            }
        }

        player->draw(window);
        player->drawHealthBar(window);

        sf::Text floorText;
        floorText.setFont(font);
        floorText.setCharacterSize(14);
        floorText.setFillColor(sf::Color::White);
        floorText.setString("Kat: " + std::to_string(currentFloor));
        floorText.setPosition(10.f, 45.f);
        window.draw(floorText);

        window.display();
    }

    return 0;
}