#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>

const int TILE_SIZE = 40;
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;

struct HaritaBolumu {
    int x, y, w, h;
};

struct Room {
    int x, y, w, h;
};

class DungeonMap {
public:
    int grid[MAP_HEIGHT][MAP_WIDTH];
    sf::RectangleShape wallTile;
    sf::RectangleShape floorTile;
    std::vector<Room> rooms;

    DungeonMap();
    void generateBSPMap();
    void draw(sf::RenderWindow& window);

private:
    void haritayiBol(HaritaBolumu bolum, int adim);
    void koridorYap(int x1, int y1, int x2, int y2);
};

#endif