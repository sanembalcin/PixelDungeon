#include "Map.h"

DungeonMap::DungeonMap() {
    wallTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    wallTile.setFillColor(sf::Color(70, 70, 80)); 
    wallTile.setOutlineThickness(-1.0f);
    wallTile.setOutlineColor(sf::Color(40, 40, 45));

    floorTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    floorTile.setFillColor(sf::Color(30, 25, 25)); 

    initMap();
}

void DungeonMap::initMap() {
    
    int taslak[MAP_HEIGHT][MAP_WIDTH] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,1},
        {1,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1},
        {1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1},
        {1,1,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,1},
        {1,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            grid[y][x] = taslak[y][x];
        }
    }
}

void DungeonMap::draw(sf::RenderWindow& window) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
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

bool DungeonMap::checkCollision(const sf::FloatRect& bounds) const {
    int startX = static_cast<int>(bounds.left / TILE_SIZE);
    int endX = static_cast<int>((bounds.left + bounds.width) / TILE_SIZE);
    int startY = static_cast<int>(bounds.top / TILE_SIZE);
    int endY = static_cast<int>((bounds.top + bounds.height) / TILE_SIZE);

    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
                if (grid[y][x] == 1) {
                    return true;
                }
            }
        }
    }
    return false;
}