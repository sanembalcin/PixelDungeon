#include "Map.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

int visualHash(int x, int y) {
    return (x * 73856093) ^ (y * 19349663);
}

void setTileSprite(sf::Sprite& sprite, sf::Texture& texture, int col, int row) {
    int tileW = texture.getSize().x / 3;
    int tileH = texture.getSize().y / 3;

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(col * tileW, row * tileH, tileW, tileH));
    sprite.setScale(
        static_cast<float>(TILE_SIZE) / tileW,
        static_cast<float>(TILE_SIZE) / tileH
    );
}

void setFullSprite(sf::Sprite& sprite, sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(
        static_cast<float>(TILE_SIZE) / texture.getSize().x,
        static_cast<float>(TILE_SIZE) / texture.getSize().y
    );
}

DungeonMap::DungeonMap() {

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    wallTexture.loadFromFile("assets/wall.png");
    floorTexture.loadFromFile("assets/floor.png");
    crackedFloorTexture.loadFromFile("assets/cracked_floor.png");
    mossFloorTexture.loadFromFile("assets/moss_floor.png");
    trapTexture.loadFromFile("assets/trap.png");
    stairsDownTexture.loadFromFile("assets/stairs_down.png");
    stairsUpTexture.loadFromFile("assets/stairs_up.png");

    setTileSprite(wallSprite, wallTexture, 1, 1);
    setTileSprite(floorSprite, floorTexture, 1, 1);
    setTileSprite(crackedFloorSprite, crackedFloorTexture, 1, 1);
    setTileSprite(mossFloorSprite, mossFloorTexture, 1, 1);
    setFullSprite(trapSprite, trapTexture);
    setFullSprite(stairsDownSprite, stairsDownTexture);
    setFullSprite(stairsUpSprite, stairsUpTexture);

    root = nullptr;

    generateNewMap();
}

DungeonMap::~DungeonMap() {
    freeTree(root);
}

void DungeonMap::freeTree(BspNode* node) {

    if (!node) return;

    freeTree(node->left);
    freeTree(node->right);

    delete node;
}

void DungeonMap::generateNewMap() {

    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            grid[y][x] = 1;
        }
    }

    if (root) {
        freeTree(root);
    }

    root = new BspNode(0, 0, MAP_WIDTH, MAP_HEIGHT);

    std::vector<BspNode*> nodes;

    nodes.push_back(root);

    for (size_t i = 0; i < nodes.size(); ++i) {

        BspNode* curr = nodes[i];

        if (curr->width > 10 || curr->height > 10) {

            if (splitNode(curr)) {

                nodes.push_back(curr->left);
                nodes.push_back(curr->right);
            }
        }
    }

    createRooms(root);

    std::vector<sf::IntRect> allRooms;
    std::vector<BspNode*> stack;

    stack.push_back(root);

    while (!stack.empty()) {

        BspNode* curr = stack.back();
        stack.pop_back();

        if (!curr) continue;

        if (!curr->left && !curr->right) {

            if (curr->room.width > 0) {
                allRooms.push_back(curr->room);
            }
        }

        else {

            stack.push_back(curr->left);
            stack.push_back(curr->right);
        }
    }

    for (size_t i = 0; i + 1 < allRooms.size(); ++i) {
        createCorridor(allRooms[i], allRooms[i + 1]);
    }

    placeStairs(false);
}

void DungeonMap::placeStairs(bool hasUpStairs) {
    bool downPlaced = false;
    bool upPlaced = false;

    while (!downPlaced) {
        int x = rand() % MAP_WIDTH;
        int y = rand() % MAP_HEIGHT;

        if (grid[y][x] == 0) {
            grid[y][x] = 4;
            downPlaced = true;
        }
    }

    if (hasUpStairs) {
        while (!upPlaced) {
            int x = rand() % MAP_WIDTH;
            int y = rand() % MAP_HEIGHT;

            if (grid[y][x] == 0) {
                grid[y][x] = 5;
                upPlaced = true;
            }
        }
    }
}

bool DungeonMap::splitNode(BspNode* node) {

    if (node->left || node->right) return false;

    bool splitHorizontal = (std::rand() % 2 == 0);

    if (node->width > node->height * 1.5)
        splitHorizontal = false;

    else if (node->height > node->width * 1.5)
        splitHorizontal = true;

    int maxSplit = (splitHorizontal ? node->height : node->width) - 4;

    if (maxSplit < 4) return false;

    int splitPoint = 4 + (std::rand() % (maxSplit - 4 + 1));

    if (splitHorizontal) {

        node->left = new BspNode(
            node->x,
            node->y,
            node->width,
            splitPoint
        );

        node->right = new BspNode(
            node->x,
            node->y + splitPoint,
            node->width,
            node->height - splitPoint
        );
    }

    else {

        node->left = new BspNode(
            node->x,
            node->y,
            splitPoint,
            node->height
        );

        node->right = new BspNode(
            node->x + splitPoint,
            node->y,
            node->width - splitPoint,
            node->height
        );
    }

    return true;
}

void DungeonMap::createRooms(BspNode* node) {

    if (!node) return;

    if (node->left || node->right) {

        createRooms(node->left);
        createRooms(node->right);
    }

    else {

        int minW = 4;
        int minH = 4;

        int w = minW + (std::rand() % (std::max(1, node->width - minW)));
        int h = minH + (std::rand() % (std::max(1, node->height - minH)));

        int maxRx = std::max(1, node->width - w - 1);
        int maxRy = std::max(1, node->height - h - 1);

        int rx = node->x + 1 + (std::rand() % maxRx);
        int ry = node->y + 1 + (std::rand() % maxRy);

        node->room = sf::IntRect(rx, ry, w, h);

        for (int y = ry; y < ry + h; ++y) {
            for (int x = rx; x < rx + w; ++x) {

                if (x >= 0 && x < MAP_WIDTH &&
                    y >= 0 && y < MAP_HEIGHT) {

                    grid[y][x] = 0;
                }
            }
        }
    }
}

void DungeonMap::createCorridor(sf::IntRect roomA, sf::IntRect roomB) {

    int startX = roomA.left + roomA.width / 2;
    int startY = roomA.top + roomA.height / 2;

    int endX = roomB.left + roomB.width / 2;
    int endY = roomB.top + roomB.height / 2;

    int currentX = startX;
    int currentY = startY;

    while (currentX != endX) {

        if (currentX >= 0 &&
            currentX < MAP_WIDTH &&
            currentY >= 0 &&
            currentY < MAP_HEIGHT) {

            grid[currentY][currentX] = 0;
        }

        if (currentX < endX)
            currentX++;
        else
            currentX--;
    }

    while (currentY != endY) {

        if (currentX >= 0 &&
            currentX < MAP_WIDTH &&
            currentY >= 0 &&
            currentY < MAP_HEIGHT) {

            grid[currentY][currentX] = 0;
        }

        if (currentY < endY)
            currentY++;
        else
            currentY--;
    }
}

void DungeonMap::draw(sf::RenderWindow& window, const sf::Vector2f& playerPos, float viewRadius) {


    for (int y = 0; y < MAP_HEIGHT; ++y) {

        for (int x = 0; x < MAP_WIDTH; ++x) {

            sf::Vector2f tilePos(x * TILE_SIZE, y * TILE_SIZE);

            float distance = std::sqrt(
                std::pow(playerPos.x - tilePos.x, 2) +
                std::pow(playerPos.y - tilePos.y, 2)
            );

            if (distance <= viewRadius) {

                int h = std::abs(visualHash(x, y));

                if (grid[y][x] == 1) {

                    wallSprite.setPosition(tilePos);
                    window.draw(wallSprite);

                    sf::RectangleShape wallShade;
                    wallShade.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                    wallShade.setFillColor(sf::Color(0, 0, 0, 95));
                    wallShade.setPosition(tilePos);
                    window.draw(wallShade);

                    sf::RectangleShape wallTopLight;
                    wallTopLight.setSize(sf::Vector2f(TILE_SIZE, 3.f));
                    wallTopLight.setFillColor(sf::Color(120, 120, 120, 90));
                    wallTopLight.setPosition(tilePos.x, tilePos.y);
                    window.draw(wallTopLight);
}

                else {

                    if (h % 7 == 0) {

                        mossFloorSprite.setPosition(tilePos);
                        window.draw(mossFloorSprite);
                    }

                    else if (h % 5 == 0) {

                        crackedFloorSprite.setPosition(tilePos);
                        window.draw(crackedFloorSprite);
                    }

                    else {

                        floorSprite.setPosition(tilePos);
                        window.draw(floorSprite);
                    }

                    if (grid[y][x] == 3) {

                        trapSprite.setPosition(tilePos);
                        window.draw(trapSprite);
                    }

                    else if (grid[y][x] == 4) {
                        stairsDownSprite.setPosition(tilePos);
                        window.draw(stairsDownSprite);
                    }
                    else if (grid[y][x] == 5) {
                        stairsUpSprite.setPosition(tilePos);
                        window.draw(stairsUpSprite);
                    }
                }
            }

            else {

                sf::RectangleShape fogTile;

                fogTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                fogTile.setFillColor(sf::Color(8, 8, 12));
                fogTile.setPosition(tilePos);

                window.draw(fogTile);
            }
        }
    }
}