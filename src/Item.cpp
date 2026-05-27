#include "Item.h"

Item::Item(ItemType type, float x, float y, const std::string& texturePath) {
    this->type = type;
    this->active = true;

    if (!texture.loadFromFile(texturePath)) {
        active = false;
        return;
    }

    sprite.setTexture(texture);

    sf::Vector2u textureSize = texture.getSize();

    if (textureSize.x > 0 && textureSize.y > 0) {
        float scaleX = 16.f / textureSize.x;
        float scaleY = 16.f / textureSize.y;
        sprite.setScale(scaleX, scaleY);
    }

    sprite.setPosition(x, y);
}

void Item::draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) {
    if (active) {
        window.draw(sprite);
    }
}

sf::FloatRect Item::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Item::isActive() const {
    return active;
}

void Item::collect() {
    active = false;
}

ItemType Item::getType() const {
    return type;
}