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
    HOW_TO_PLAY,
    CHARACTER_SELECT,
    PLAYING,
    PLAYER_DEAD,
    GAME_OVER
};

sf::Vector2f findFirstFloorTile(DungeonMap& map) {
    for (int y = 2; y < MAP_HEIGHT - 2; ++y) {
        for (int x = 2; x < MAP_WIDTH - 2; ++x) {
            if (map.grid[y][x] == 0 &&
                map.grid[y - 1][x] == 0 &&
                map.grid[y + 1][x] == 0 &&
                map.grid[y][x - 1] == 0 &&
                map.grid[y][x + 1] == 0 &&
                map.grid[y - 1][x - 1] == 0 &&
                map.grid[y - 1][x + 1] == 0 &&
                map.grid[y + 1][x - 1] == 0 &&
                map.grid[y + 1][x + 1] == 0) {

                return sf::Vector2f(
                    x * TILE_SIZE + 8.f,
                    y * TILE_SIZE + 8.f
                );
            }
        }
    }

    return sf::Vector2f(3 * TILE_SIZE + 8.f, 3 * TILE_SIZE + 8.f);
}

sf::Vector2f findLastFloorTile(DungeonMap& map) {
    for (int y = MAP_HEIGHT - 2; y >= 1; --y) {
        for (int x = MAP_WIDTH - 2; x >= 1; --x) {
            if (map.grid[y][x] == 0 &&
                map.grid[y - 1][x] == 0 &&
                map.grid[y + 1][x] == 0 &&
                map.grid[y][x - 1] == 0 &&
                map.grid[y][x + 1] == 0) {

                return sf::Vector2f(
                    x * TILE_SIZE + 8.f,
                    y * TILE_SIZE + 8.f
                );
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
    int heartCounter = 0;
    int speedCounter = 0;
    int swordCounter = 0;
    int mindCounter = 0;

    for (int y = 1; y < MAP_HEIGHT - 1; ++y) {
        for (int x = 1; x < MAP_WIDTH - 1; ++x) {
            if (map.grid[y][x] == 0) {
                if (rand() % 100 < 6 && itemCounter < 9) {
                    float px = x * TILE_SIZE + 8.f;
                    float py = y * TILE_SIZE + 8.f;
                    int r = rand() % 4;

                    if (r == 0 && heartCounter < 4) {
                        worldItems.emplace_back(ItemType::HEART, px, py, "assets/heart.png");
                        heartCounter++;
                        itemCounter++;
                    }
                    else if (r == 1 && speedCounter < 3) {
                        worldItems.emplace_back(ItemType::SPEED, px, py, "assets/speed.png");
                        speedCounter++;
                        itemCounter++;
                    }
                    else if (r == 2 && swordCounter < 1 && rand() % 100 < 25) {
                        worldItems.emplace_back(ItemType::SWORD, px, py, "assets/sword.png");
                        swordCounter++;
                        itemCounter++;
                    }
                    else if (r == 3 && mindCounter < 1 && rand() % 100 < 80) {
                        worldItems.emplace_back(ItemType::MIND, px, py, "assets/mind.png");
                        mindCounter++;
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

    sf::RenderWindow window(sf::VideoMode(800, 700), "Pixel Dungeon");
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
    sf::Clock deathClock;

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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
                gameState = GameState::HOW_TO_PLAY;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
                std::ifstream saveFile("save.txt");

                if (saveFile.is_open()) {
                    float px;
                    float py;
                    int hp;
                    int maxHp;
                    int atk;
                    float spd;
                    float vr;
                    int heartCount;
                    int speedCount;
                    int swordCount;
                    int mindCount;
                    int floor;

                    saveFile >> px >> py;
                    saveFile >> hp;
                    saveFile >> maxHp;
                    saveFile >> atk;
                    saveFile >> spd;
                    saveFile >> vr;
                    saveFile >> heartCount;
                    saveFile >> speedCount;
                    saveFile >> swordCount;
                    saveFile >> mindCount;
                    saveFile >> floor;

                    for (int y = 0; y < MAP_HEIGHT; y++) {
                        for (int x = 0; x < MAP_WIDTH; x++) {
                            saveFile >> map.grid[y][x];
                        }
                    }

                    player->setMaxHealth(maxHp);
                    player->setHealth(hp);
                    player->setAttackPower(atk);
                    player->setSpeed(spd);
                    player->setViewRadius(vr);
                    player->setInventoryCounts(heartCount, speedCount, swordCount, mindCount);
                    player->setPosition(px, py);

                    currentFloor = floor;

                    gameState = GameState::PLAYING;

                    saveFile.close();
                }
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
            startText.setString("ENTER - YENI OYUN");
            startText.setCharacterSize(22);
            startText.setFillColor(sf::Color::White);
            startText.setPosition(250.f, 300.f);

            sf::Text loadText;
            loadText.setFont(font);
            loadText.setString("L - KAYIT YUKLE");
            loadText.setCharacterSize(22);
            loadText.setFillColor(sf::Color::White);
            loadText.setPosition(250.f, 350.f);

            sf::Text helpText;
            helpText.setFont(font);
            helpText.setString("H - NASIL OYNANIR");
            helpText.setCharacterSize(22);
            helpText.setFillColor(sf::Color::White);
            helpText.setPosition(250.f, 400.f);

            sf::Text exitText;
            exitText.setFont(font);
            exitText.setString("ESC - CIKIS");
            exitText.setCharacterSize(22);
            exitText.setFillColor(sf::Color::White);
            exitText.setPosition(270.f, 450.f);

            window.draw(title);
            window.draw(startText);
            window.draw(loadText);
            window.draw(helpText);
            window.draw(exitText);

             window.display();

            continue;
        }

        if (gameState == GameState::HOW_TO_PLAY) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
            gameState = GameState::MENU;
        }

        window.clear(sf::Color(10, 10, 20));

        sf::Text title;
        title.setFont(font);
        title.setString("HOW TO PLAY");
        title.setCharacterSize(36);
        title.setFillColor(sf::Color(180, 180, 255));
        title.setPosition(250.f, 60.f);

        sf::Text controls;
        controls.setFont(font);
        controls.setCharacterSize(18);
        controls.setFillColor(sf::Color::White);
        controls.setString(
            "W A S D  - Hareket\n"
            "SPACE    - Saldiri\n"
            "LEFT SHIFT - Rogue Dash\n"
            "ENTER    - Merdiven kullan\n"
            "P        - Oyunu kaydet\n"
            "L        - Oyunu yukle\n"
            "ESC      - Menuye don\n\n"
            "Esyalar:\n"
            "Heart        - Can yeniler veya maksimum can arttirir\n"
            "Speed Potion - Hareket hizini arttirir\n"
            "Sword        - Saldiri gucunu arttirir\n"
            "Mind Orb     - Gorus alanini arttirir\n\n"
            "Amac:\n"
            "Zindanlari kesfet, dusmanlari yen,\n"
            "esyalar topla ve hayatta kal."
        );

        controls.setPosition(120.f, 130.f);

        window.draw(title);
        window.draw(controls);

        sf::Text backText;
        backText.setFont(font);
        backText.setString("B - MENUYE DON");
        backText.setCharacterSize(18);
        backText.setFillColor(sf::Color(180, 180, 180));
        backText.setPosition(240.f, 620.f);
        window.draw(backText);

        window.display();

        continue;
    }

        if (gameState == GameState::CHARACTER_SELECT) {

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
                selectedCharacter = 0;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
                selectedCharacter = 1;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {

                if (selectedCharacter == 0) {
                    player = std::make_unique<Warrior>();
             }

                else {
                    player = std::make_unique<Rogue>();
                }

            player->setPosition(playerStart.x, playerStart.y);

            gameState = GameState::PLAYING;
            }

            window.clear(sf::Color(15, 15, 25));

            sf::Text title;
            title.setFont(font);
            title.setString("KARAKTERINI SEC");
            title.setCharacterSize(30);
            title.setFillColor(sf::Color::White);
            title.setPosition(120.f, 60.f);

            sf::Texture warriorPreviewTex;
            warriorPreviewTex.loadFromFile("assets/warrior_down.png");

            sf::Sprite warriorPreview;
            warriorPreview.setTexture(warriorPreviewTex);

            sf::Vector2u wSize = warriorPreviewTex.getSize();

            if (wSize.x > 0 && wSize.y > 0) {

                warriorPreview.setScale(
                96.f / wSize.x,
                96.f / wSize.y
                );
            }

            warriorPreview.setPosition(180.f, 220.f);

            sf::Texture roguePreviewTex;
            roguePreviewTex.loadFromFile("assets/rogue_down.png");

            sf::Sprite roguePreview;
            roguePreview.setTexture(roguePreviewTex);

            sf::Vector2u rSize = roguePreviewTex.getSize();

            if (rSize.x > 0 && rSize.y > 0) {

                roguePreview.setScale(
                96.f / rSize.x,
                96.f / rSize.y
                );
            }

            roguePreview.setPosition(500.f, 220.f);

            if (selectedCharacter == 0) {
                warriorPreview.setColor(sf::Color::White);
                roguePreview.setColor(sf::Color(120, 120, 120));
            }

            else {
                roguePreview.setColor(sf::Color::White);
                warriorPreview.setColor(sf::Color(120, 120, 120));
            }

            sf::Text warriorText;
            warriorText.setFont(font);
            warriorText.setString("1 - WARRIOR");
            warriorText.setCharacterSize(18);
            warriorText.setFillColor(sf::Color::White);
            warriorText.setPosition(150.f, 360.f);

            sf::Text rogueText;
            rogueText.setFont(font);
            rogueText.setString("2 - ROGUE");
            rogueText.setCharacterSize(18);
            rogueText.setFillColor(sf::Color::White);
            rogueText.setPosition(500.f, 360.f);

            sf::Text enterText;
            enterText.setFont(font);
            enterText.setString("SPACE - BASLA");
            enterText.setCharacterSize(18);
            enterText.setFillColor(sf::Color(180, 180, 255));
            enterText.setPosition(260.f, 500.f);

            window.draw(title);

            window.draw(warriorPreview);
            window.draw(roguePreview);

            window.draw(warriorText);
            window.draw(rogueText);
            window.draw(enterText);

            window.display();

            continue;
        }

        if (gameState == GameState::PLAYER_DEAD) {
            window.clear(sf::Color(10, 0, 0));

            map.draw(window, player->getPosition(), player->getViewRadius());
            player->draw(window);

            if (deathClock.getElapsedTime().asSeconds() >= 1.5f) {
                gameState = GameState::GAME_OVER;
            }

            window.display();

            continue;
        }

        if (gameState == GameState::GAME_OVER) {

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {

                currentFloor = 1;

                map.generateNewMap();

                spawnItemsAndTraps(map, worldItems);

                if (selectedCharacter == 0) {
                    player = std::make_unique<Warrior>();
                }

                else {
                    player = std::make_unique<Rogue>();
                }

                sf::Vector2f newPlayerPos = findFirstFloorTile(map);
                sf::Vector2f newSlimePos = findLastFloorTile(map);

                player->setPosition(newPlayerPos.x, newPlayerPos.y);

                slime.reset(newSlimePos.x, newSlimePos.y);

                skeleton.setPosition(-9999.f, -9999.f);

                mage = MageEnemy(-9999.f, -9999.f);

                slimePotionSpawned = false;

                gameState = GameState::PLAYING;
            }

            window.clear(sf::Color(10, 0, 0));

            player->draw(window);

            sf::Text overText;
            overText.setFont(font);
            overText.setString("GAME OVER");
            overText.setCharacterSize(52);
            overText.setFillColor(sf::Color(220, 40, 40));
            overText.setPosition(170.f, 160.f);

            sf::Text retryText;
            retryText.setFont(font);
            retryText.setString("R - RESTART");
            retryText.setCharacterSize(24);
            retryText.setFillColor(sf::Color::White);
            retryText.setPosition(250.f, 330.f);

            sf::Text exitText;
            exitText.setFont(font);
            exitText.setString("ESC - QUIT");
            exitText.setCharacterSize(24);
            exitText.setFillColor(sf::Color::White);
            exitText.setPosition(270.f, 380.f);

            window.draw(overText);

            window.draw(retryText);

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
                saveFile << player->getMaxHealth() << "\n";
                saveFile << player->getAttackPower() << "\n";
                saveFile << player->getSpeed() << "\n";
                saveFile << player->getViewRadius() << "\n";

                saveFile << player->getItemCount(ItemType::HEART) << "\n";
                saveFile << player->getItemCount(ItemType::SPEED) << "\n";
                saveFile << player->getItemCount(ItemType::SWORD) << "\n";
                saveFile << player->getItemCount(ItemType::MIND) << "\n";

                saveFile << currentFloor << "\n";
                for (int y = 0; y < MAP_HEIGHT; y++) {
                    for (int x = 0; x < MAP_WIDTH; x++) {
                        saveFile << map.grid[y][x] << " ";
                    }
                    saveFile << "\n";
                }

                saveFile.close();
            }
        }

        if (lPressed && !lWasPressed) {
            std::ifstream saveFile("save.txt");

            if (saveFile.is_open()) {
                float px;
                float py;
                int hp;
                int maxHp;
                int atk;
                float spd;
                float vr;
                int heartCount;
                int speedCount;
                int swordCount;
                int mindCount;
                int floor;

                saveFile >> px >> py;
                saveFile >> hp;
                saveFile >> maxHp;
                saveFile >> atk;
                saveFile >> spd;
                saveFile >> vr;
                saveFile >> heartCount;
                saveFile >> speedCount;
                saveFile >> swordCount;
                saveFile >> mindCount;
                saveFile >> floor;

                for (int y = 0; y < MAP_HEIGHT; y++) {
                    for (int x = 0; x < MAP_WIDTH; x++) {
                        saveFile >> map.grid[y][x];
                    }
                }

                player->setMaxHealth(maxHp);
                player->setHealth(hp);
                player->setAttackPower(atk);
                player->setSpeed(spd);
                player->setViewRadius(vr);
                player->setInventoryCounts(heartCount, speedCount, swordCount, mindCount);
                int tileX = static_cast<int>((px + 16.f) / TILE_SIZE);
                int tileY = static_cast<int>((py + 16.f) / TILE_SIZE);

                if (tileX >= 0 && tileX < MAP_WIDTH && tileY >= 0 && tileY < MAP_HEIGHT && map.grid[tileY][tileX] != 1) {
                    float hitboxLeft = px + 10.f;
float hitboxTop = py + 14.f;
float hitboxWidth = 12.f;
float hitboxHeight = 14.f;

int leftTile = static_cast<int>(hitboxLeft / TILE_SIZE);
int rightTile = static_cast<int>((hitboxLeft + hitboxWidth) / TILE_SIZE);
int topTile = static_cast<int>(hitboxTop / TILE_SIZE);
int bottomTile = static_cast<int>((hitboxTop + hitboxHeight) / TILE_SIZE);

bool safePosition = true;

if (leftTile < 0 || rightTile >= MAP_WIDTH || topTile < 0 || bottomTile >= MAP_HEIGHT) {
    safePosition = false;
}
else {
    for (int y = topTile; y <= bottomTile; ++y) {
        for (int x = leftTile; x <= rightTile; ++x) {
            if (map.grid[y][x] == 1) {
                safePosition = false;
            }
        }
    }
}

if (safePosition) {
    player->setPosition(px, py);
}
else {
    sf::Vector2f safePos = findFirstFloorTile(map);
    player->setPosition(safePos.x, safePos.y);
}
                }
                else {
                    sf::Vector2f safePos = findFirstFloorTile(map);
                    player->setPosition(safePos.x, safePos.y);
                }

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

        if (gameState != GameState::PLAYING)
        continue;

        player->handleInput(map);
        player->update(map);
        if (player->getHealth() <= 0) {
            player->die();
            deathClock.restart();
            gameState = GameState::PLAYER_DEAD;
        }

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
                    map.placeStairs(currentFloor > 1);

                    sf::Vector2f newPlayerPos = findFirstFloorTile(map);
                    sf::Vector2f newSlimePos = findLastFloorTile(map);
                    sf::Vector2f newSkeletonPos = findMiddleFloorTile(map);
                    sf::Vector2f newMagePos = findMiddleFloorTile(map);

                    player->setPosition(newPlayerPos.x, newPlayerPos.y);
                    slime.reset(newSlimePos.x, newSlimePos.y);

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

        if (map.grid[pTileY][pTileX] == 5) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && currentFloor > 1) {
                currentFloor--;

                map.generateNewMap();
                spawnItemsAndTraps(map, worldItems);
                map.placeStairs(currentFloor > 1);

                sf::Vector2f newPlayerPos = findFirstFloorTile(map);
                sf::Vector2f newSlimePos = findLastFloorTile(map);
                sf::Vector2f newSkeletonPos = findMiddleFloorTile(map);
                sf::Vector2f newMagePos = findMiddleFloorTile(map);

                player->setPosition(newPlayerPos.x, newPlayerPos.y);
                slime.reset(newSlimePos.x, newSlimePos.y);

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
            worldItems.emplace_back(ItemType::HEART, lastSlimePos.x + 8.f, lastSlimePos.y + 8.f, "assets/heart.png");
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
                slime.takeDamage(player->getAttackPower());

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
                skeleton.takeDamage(player->getAttackPower());

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
                mage.takeDamage(player->getAttackPower());
                playerAttackClock.restart();
            }
        }

        if (!slime.isDead() && player->getBounds().intersects(slime.getBounds())) {
            player->takeDamage(5);
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

        map.draw(window, player->getPosition(),player->getViewRadius());

        for (size_t i = 0; i < worldItems.size(); ++i) {
            sf::FloatRect itemBounds = worldItems[i].getBounds();
            sf::Vector2f itemPos(itemBounds.left, itemBounds.top);
            sf::Vector2f pPos = player->getPosition();

            float distToItem = std::sqrt(
                std::pow(pPos.x - itemPos.x, 2) +
                std::pow(pPos.y - itemPos.y, 2)
            );

            float viewRadius = player->getViewRadius();

            if (distToItem <= player->getViewRadius()) {
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

            float viewRadius = player->getViewRadius();

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

            float viewRadius = player->getViewRadius();

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

            float viewRadius = player->getViewRadius();

            if (distToMage <= viewRadius) {
                mage.draw(window);
            }
        }

        player->draw(window);
        player->drawHealthBar(window);

        sf::Text floorText;
        floorText.setFont(font);
        floorText.setCharacterSize(15);
        floorText.setFillColor(sf::Color::White);

        floorText.setString("Kat: " + std::to_string(currentFloor));
        floorText.setPosition(690.f, 635.f);
        window.draw(floorText);

        window.display();
    }

    return 0;
}