#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <vector>

const int MAP_WIDTH = 25;
const int MAP_HEIGHT = 19;
const int TILE_SIZE = 32;

struct BspNode {
    int x, y, width, height;
    sf::IntRect room;
    BspNode* left;
    BspNode* right;

    BspNode(int nx, int ny, int nw, int nh) 
        : x(nx), y(ny), width(nw), height(nh), room(0, 0, 0, 0), left(nullptr), right(nullptr) {}
};

class DungeonMap {
private:
    BspNode* root;
    sf::RectangleShape wallTile;
    sf::RectangleShape floorTile;
    sf::Texture trapTexture;
    sf::Sprite trapSprite;

    void freeTree(BspNode* node);
    bool splitNode(BspNode* node);
    void createRooms(BspNode* node);
    void createCorridor(sf::IntRect roomA, sf::IntRect roomB);

public:
    int grid[MAP_HEIGHT][MAP_WIDTH];

    DungeonMap();
    ~DungeonMap();
    void generateNewMap();
    void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos);
};

#endif