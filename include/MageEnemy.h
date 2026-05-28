#ifndef MAGEENEMY_H
#define MAGEENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Map.h"
#include "Projectile.h"

class MageEnemy {
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
    bool dead;
    bool flashing;

    sf::Clock flashClock;
    sf::Clock attackClock;

    std::vector<Projectile> projectiles;

public:
    MageEnemy(float x, float y);

    void update(const sf::Vector2f& playerPos, const DungeonMap& map);
    void draw(sf::RenderWindow& window);

    void takeDamage(int amount);

    bool isDead() const;

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

    std::vector<Projectile>& getProjectiles();
};

#endif