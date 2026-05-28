#ifndef SKELETON_H
#define SKELETON_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <vector>

class Skeleton {
private:
    sf::Sprite sprite;
    sf::Texture texUp;
    sf::Texture texDown;
    sf::Texture texLeft;
    sf::Texture texRight;
    sf::Texture atkUp;
    sf::Texture atkDown;
    sf::Texture atkLeft;
    sf::Texture atkRight;
    sf::Texture deadTexture;

    float speed;
    int health;
    int damage;
    bool dead;
    bool attacking;
    bool flashing;

    sf::Clock attackClock;
    sf::Clock aiClock;
    sf::Clock flashClock;

    float attackCooldown;
    int direction;

    std::vector<sf::Vector2i> path;

public:
    Skeleton(float x, float y);

    void update(const sf::Vector2f& playerPos, const DungeonMap& map);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    void setPosition(float x, float y);

    void takeDamage(int amount);
    bool isDead() const;
    int getDamage() const;
    bool canAttack() const;
    bool tryAttack();

    std::vector<sf::Vector2i> findPath(const sf::Vector2i& start, const sf::Vector2i& target, const DungeonMap& map);
};

#endif