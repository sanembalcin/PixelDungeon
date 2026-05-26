#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>

const int TILE_SIZE = 32;
const int MAP_WIDTH = 25;
const int MAP_HEIGHT = 19;

struct BspNode {
    int x, y, width, height;
    BspNode* left;
    BspNode* right;
    sf::IntRect room;

    BspNode(int x, int y, int w, int h) 
        : x(x), y(y), width(w), height(h), left(nullptr), right(nullptr), room(0, 0, 0, 0) {}
};

class DungeonMap {
private:
    sf::RectangleShape wallTile;
    sf::RectangleShape floorTile;
    BspNode* root;

    bool splitNode(BspNode* node);
    void createRooms(BspNode* node);
    void createCorridor(sf::IntRect roomA, sf::IntRect roomB);

public:
    int grid[MAP_HEIGHT][MAP_WIDTH];
    DungeonMap();
    ~DungeonMap();
    void generateNewMap();
    void draw(sf::RenderWindow& window);
    void freeTree(BspNode* node);
};

#endif