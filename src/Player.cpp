#include "Player.h"
#include <iostream>

Player::Player() {
    speed = 2.0f;
    hp = 100;
    attack = 10;
}

void Player::update(DungeonMap& map) {}
void Player::draw(sf::RenderWindow& window) {}


Warrior::Warrior() {
    speed = 2.5f;
    hp = 150;
    attack = 15;
    sprite.setPosition(60.0f, 60.0f);

    
    if (texture.loadFromFile("assets/warrior_down.png")) {
        sprite.setTexture(texture);
        
       
        sprite.setScale(0.12f, 0.12f); 
    } else {
        std::cout << "[HATA] assets/warrior_down.png yuklenemedi!" << std::endl;
    }
}

void Warrior::update(DungeonMap& map) {
    sf::Vector2f nextPos = sprite.getPosition();
    bool hareketVar = false;

  
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        nextPos.y -= speed;
        if (texture.loadFromFile("assets/warrior_up.png")) {
            sprite.setTexture(texture);
        }
        hareketVar = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        nextPos.y += speed;
        if (texture.loadFromFile("assets/warrior_down.png")) {
            sprite.setTexture(texture);
        }
        hareketVar = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        nextPos.x -= speed;
        if (texture.loadFromFile("assets/warrior_left.png")) {
            sprite.setTexture(texture);
        }
        hareketVar = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        nextPos.x += speed;
        if (texture.loadFromFile("assets/warrior_right.png")) {
            sprite.setTexture(texture);
        }
        hareketVar = true;
    }

    if (hareketVar) {
        
        int tileX = static_cast<int>((nextPos.x + 10) / TILE_SIZE);
        int tileY = static_cast<int>((nextPos.y + 10) / TILE_SIZE);

        if (tileX >= 0 && tileX < MAP_WIDTH && tileY >= 0 && tileY < MAP_HEIGHT) {
            if (map.grid[tileY][tileX] == 0) {
                sprite.setPosition(nextPos);
            }
        }
    }
}

void Warrior::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}