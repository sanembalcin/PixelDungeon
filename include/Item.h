#ifndef ITEM_H
#define ITEM_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include <string>

class Item {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    ItemType type;
    bool active;

public:
    Item(ItemType itemType, float x, float y, const std::string& texturePath);
    void draw(sf::RenderWindow& window, const sf::Vector2f& playerPos);
    sf::FloatRect getBounds() const;
    bool isActive() const;
    void collect();
    ItemType getType() const;
};

#endif