#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <SFML/Graphics.hpp>
#include "Map.h"

class Projectile {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    bool active;
    int damage;

public:
    Projectile(float x, float y, sf::Vector2f direction, const std::string& texturePath);

    void update(const DungeonMap& map);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    bool isActive() const;
    void deactivate();
    int getDamage() const;
};

#endif