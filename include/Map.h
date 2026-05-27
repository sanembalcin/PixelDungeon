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

    sf::Texture wallTexture;
    sf::Texture floorTexture;
    sf::Texture crackedFloorTexture;
    sf::Texture mossFloorTexture;
    sf::Texture trapTexture;
    sf::Texture stairTexture;

    sf::Sprite wallSprite;
    sf::Sprite floorSprite;
    sf::Sprite crackedFloorSprite;
    sf::Sprite mossFloorSprite;
    sf::Sprite trapSprite;
    sf::Sprite stairSprite;

    void freeTree(BspNode* node);
    bool splitNode(BspNode* node);
    void createRooms(BspNode* node);
    void createCorridor(sf::IntRect roomA, sf::IntRect roomB);

public:
    int grid[MAP_HEIGHT][MAP_WIDTH];

    DungeonMap();
    ~DungeonMap();
    void generateNewMap();
    void placeStairs();
    void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos);
};

#endif