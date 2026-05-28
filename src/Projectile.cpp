#include "Projectile.h"
#include <cmath>
#include <string>

Projectile::Projectile(float x, float y, sf::Vector2f direction, const std::string& texturePath) {
    texture.loadFromFile(texturePath);
    sprite.setTexture(texture);

    sf::Vector2u size = texture.getSize();

    if (size.x > 0 && size.y > 0) {
        sprite.setScale(18.f / size.x, 18.f / size.y);
    }

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length != 0) {
        direction /= length;
    }

    velocity = direction * 4.5f;
    active = true;
    damage = 20;

    sprite.setPosition(x, y);
}

void Projectile::update(const DungeonMap& map) {
    if (!active) return;

    sprite.move(velocity);

    sf::Vector2f pos = sprite.getPosition();

    int tileX = static_cast<int>((pos.x + 9.f) / TILE_SIZE);
    int tileY = static_cast<int>((pos.y + 9.f) / TILE_SIZE);

    if (tileX < 0 || tileX >= MAP_WIDTH || tileY < 0 || tileY >= MAP_HEIGHT) {
        active = false;
        return;
    }

    if (map.grid[tileY][tileX] == 1) {
        active = false;
    }
}

void Projectile::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(sprite);
    }
}

sf::FloatRect Projectile::getBounds() const {
    return sprite.getGlobalBounds();
}

bool Projectile::isActive() const {
    return active;
}

void Projectile::deactivate() {
    active = false;
}

int Projectile::getDamage() const {
    return damage;
}