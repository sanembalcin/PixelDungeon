#include "Potion.h"

Potion::Potion(float x, float y) {
    texture.loadFromFile("assets/potion.png");
    sprite.setTexture(texture);
    
    sf::Vector2u textureSize = texture.getSize();
    if (textureSize.x > 0 && textureSize.y > 0) {
        float scaleX = 16.f / textureSize.x;
        float scaleY = 16.f / textureSize.y;
        sprite.setScale(scaleX, scaleY);
    }

    sprite.setPosition(x, y);
    active = true;
}

void Potion::draw(sf::RenderWindow& window, const sf::Vector2f& playerPos) {
    if (!active) return;
    window.draw(sprite);
}

sf::FloatRect Potion::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Potion::isActive() const {
    return active;
}

void Potion::collect() {
    active = false;
}