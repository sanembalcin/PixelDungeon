#ifndef POTION_H
#define POTION_H

#include <SFML/Graphics.hpp>

class Potion {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    bool active;

public:
    Potion(float x, float y);
    void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos);
    sf::FloatRect getBounds() const;
    bool isActive() const;
    void collect();
};

#endif