#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>

const int TILE_SIZE = 40;
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;

class DungeonMap {
public:
    int grid[MAP_HEIGHT][MAP_WIDTH];
    sf::RectangleShape wallTile;
    sf::RectangleShape floorTile;

    DungeonMap();
    void initMap();
    void draw(sf::RenderWindow& window);
    bool checkCollision(const sf::FloatRect& bounds) const;
};

#endif