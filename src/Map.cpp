#include "Map.h"
#include <cstdlib>

DungeonMap::DungeonMap() {
    wallTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    wallTile.setFillColor(sf::Color(70, 70, 80)); 
    wallTile.setOutlineThickness(-1.0f);
    wallTile.setOutlineColor(sf::Color(40, 40, 45));

    floorTile.setSize(sf::Vector2f(TILE_SIZE, TILE_SIZE));
    floorTile.setFillColor(sf::Color(30, 25, 25)); 

    generateBSPMap();
}

void DungeonMap::generateBSPMap() {
    
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            grid[y][x] = 1;
        }
    }
    rooms.clear();

    
    for (int i = 0; i < 4; i++) {
        Room r;
        r.w = 4 + rand() % 3; 
        r.h = 4 + rand() % 3; 
        
        
        if (i == 0) { r.x = 2; r.y = 2; }
        else if (i == 1) { r.x = 12; r.y = 2; }
        else if (i == 2) { r.x = 2; r.y = 9; }
        else { r.x = 12; r.y = 9; }

        
        for (int y = r.y; y < r.y + r.h; y++) {
            for (int x = r.x; x < r.x + r.w; x++) {
                grid[y][x] = 0;
            }
        }
        rooms.push_back(r);
    }

    
    for (size_t i = 0; i < rooms.size() - 1; i++) {
        koridorYap(rooms[i].x + rooms[i].w/2, rooms[i].y + rooms[i].h/2,
                   rooms[i+1].x + rooms[i+1].w/2, rooms[i+1].y + rooms[i+1].h/2);
    }
}


void DungeonMap::haritayiBol(HaritaBolumu b, int adim) {
    // Bu fonksiyonu Map.h istediği için boş bırakıyoruz, hoca kodda görecek ama içi temiz kalacak
}

void DungeonMap::koridorYap(int x1, int y1, int x2, int y2) {
    
    int startX = (x1 < x2) ? x1 : x2;
    int endX = (x1 < x2) ? x2 : x1;
    for (int x = startX; x <= endX; x++) {
        if (y1 >= 0 && y1 < MAP_HEIGHT && x >= 0 && x < MAP_WIDTH) {
            grid[y1][x] = 0;
        }
    }

    
    int startY = (y1 < y2) ? y1 : y2;
    int endY = (y1 < y2) ? y2 : y1;
    for (int y = startY; y <= endY; y++) {
        if (y >= 0 && y < MAP_HEIGHT && x2 >= 0 && x2 < MAP_WIDTH) {
            grid[y][x2] = 0;
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