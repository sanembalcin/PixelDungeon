#include "Map.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

DungeonMap::DungeonMap() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    wallTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    wallTile.setFillColor(sf::Color(80, 80, 90));
    floorTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    floorTile.setFillColor(sf::Color(30, 30, 35));
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
        } else {
            stack.push_back(curr->left);
            stack.push_back(curr->right);
        }
    }

    for (size_t i = 0; i + 1 < allRooms.size(); ++i) {
        createCorridor(allRooms[i], allRooms[i + 1]);
    }
}

bool DungeonMap::splitNode(BspNode* node) {
    if (node->left || node->right) return false;

    bool splitHorizontal = (std::rand() % 2 == 0);
    if (node->width > node->height * 1.5) splitHorizontal = false;
    else if (node->height > node->width * 1.5) splitHorizontal = true;

    int maxSplit = (splitHorizontal ? node->height : node->width) - 4;
    if (maxSplit < 4) return false;

    int splitPoint = 4 + (std::rand() % (maxSplit - 4 + 1));

    if (splitHorizontal) {
        node->left = new BspNode(node->x, node->y, node->width, splitPoint);
        node->right = new BspNode(node->x, node->y + splitPoint, node->width, node->height - splitPoint);
    } else {
        node->left = new BspNode(node->x, node->y, splitPoint, node->height);
        node->right = new BspNode(node->x + splitPoint, node->y, node->width - splitPoint, node->height);
    }

    return true;
}

void DungeonMap::createRooms(BspNode* node) {
    if (!node) return;

    if (node->left || node->right) {
        createRooms(node->left);
        createRooms(node->right);
    } else {
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
                if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
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
        if (currentX >= 0 && currentX < MAP_WIDTH && currentY >= 0 && currentY < MAP_HEIGHT) {
            grid[currentY][currentX] = 0;
        }
        if (currentX < endX) currentX++;
        else currentX--;
    }

    while (currentY != endY) {
        if (currentX >= 0 && currentX < MAP_WIDTH && currentY >= 0 && currentY < MAP_HEIGHT) {
            grid[currentY][currentX] = 0;
        }
        if (currentY < endY) currentY++;
        else currentY--;
    }
}

void DungeonMap::draw(sf::RenderWindow& window) {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            if (grid[y][x] == 1) {
                wallTile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                window.draw(wallTile);
            } else {
                floorTile.setPosition(x * TILE_SIZE, y * TILE_SIZE);
                window.draw(floorTile);
            }
        }
    }
}