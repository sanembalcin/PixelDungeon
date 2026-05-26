#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <queue>

class DungeonMap;

class Enemy {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
    int health;
    bool isChasing;

    std::vector<sf::Vector2i> findPath(const sf::Vector2i& start, const sf::Vector2i& target, const DungeonMap& map);

public:
    Enemy(float startX, float startY);
    void update(const sf::Vector2f& playerPos, const DungeonMap& map);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void takeDamage(int amount);
    int getHealth() const;
};

#endif