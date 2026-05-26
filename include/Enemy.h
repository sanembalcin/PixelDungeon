#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <vector>

class Enemy {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
    int health;
    std::vector<sf::Vector2i> path;

public:
    Enemy(float x, float y);
    void update(const sf::Vector2f& playerPos, const DungeonMap& map);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    void takeDamage(int amount);
    bool isDead() const;
    void setPosition(float x, float y);
    int getHealth() const;
    std::vector<sf::Vector2i> findPath(const sf::Vector2i& start, const sf::Vector2i& target, const DungeonMap& map);
};

#endif