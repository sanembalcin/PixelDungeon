#include "MageEnemy.h"
#include <cmath>

MageEnemy::MageEnemy(float x, float y) {
    texUp.loadFromFile("assets/mage_up.png");
    texDown.loadFromFile("assets/mage_down.png");
    texLeft.loadFromFile("assets/mage_left.png");
    texRight.loadFromFile("assets/mage_right.png");

    atkUp.loadFromFile("assets/mage_attack_up.png");
    atkDown.loadFromFile("assets/mage_attack_down.png");
    atkLeft.loadFromFile("assets/mage_attack_left.png");
    atkRight.loadFromFile("assets/mage_attack_right.png");

    deadTexture.loadFromFile("assets/mage_dead.png");

    sprite.setTexture(texDown);

    sf::Vector2u size = texDown.getSize();

    if (size.x > 0 && size.y > 0) {
        sprite.setScale(32.f / size.x, 32.f / size.y);
    }

    sprite.setPosition(x, y);

    speed = 1.5f;
    health = 150;
    dead = false;
    flashing = false;
}

void MageEnemy::update(const sf::Vector2f& playerPos, const DungeonMap& map) {
    if (dead) return;

    if (flashing && flashClock.getElapsedTime().asSeconds() >= 0.15f) {
        sprite.setColor(sf::Color::White);
        flashing = false;
    }

    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f diff = playerPos - pos;

    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (distance < 120.f) {
        if (distance != 0) {
            diff /= distance;
        }

        sf::Vector2f oldPos = sprite.getPosition();

        sprite.move(-diff * speed);

        sf::Vector2f newPos = sprite.getPosition();

        float hitboxLeft = newPos.x + 8.f;
        float hitboxTop = newPos.y + 8.f;
        float hitboxWidth = 16.f;
        float hitboxHeight = 16.f;

        int leftTile = static_cast<int>(hitboxLeft / TILE_SIZE);
        int rightTile = static_cast<int>((hitboxLeft + hitboxWidth) / TILE_SIZE);
        int topTile = static_cast<int>(hitboxTop / TILE_SIZE);
        int bottomTile = static_cast<int>((hitboxTop + hitboxHeight) / TILE_SIZE);

        bool blocked = false;

        if (leftTile < 0 || rightTile >= MAP_WIDTH || topTile < 0 || bottomTile >= MAP_HEIGHT) {
            blocked = true;
        }
        else {
            for (int y = topTile; y <= bottomTile; ++y) {
                for (int x = leftTile; x <= rightTile; ++x) {
                    if (map.grid[y][x] == 1) {
                        blocked = true;
                    }
                }
            }
        }

        if (blocked) {
            sprite.setPosition(oldPos);
        }

        if (std::abs(diff.x) > std::abs(diff.y)) {
            if (diff.x > 0)
                sprite.setTexture(texLeft);
            else
                sprite.setTexture(texRight);
        }
        else {
            if (diff.y > 0)
                sprite.setTexture(texUp);
            else
                sprite.setTexture(texDown);
        }
    }
    else if (distance < 260.f) {
        if (attackClock.getElapsedTime().asSeconds() >= 1.5f) {
            sf::Vector2f dir = playerPos - pos;

            if (std::abs(dir.x) > std::abs(dir.y)) {
                if (dir.x > 0)
                    sprite.setTexture(atkRight);
                else
                    sprite.setTexture(atkLeft);
            }
            else {
                if (dir.y > 0)
                    sprite.setTexture(atkDown);
                else
                    sprite.setTexture(atkUp);
            }

            projectiles.emplace_back(
                pos.x + 16.f,
                pos.y + 16.f,
                dir,
                "assets/magic_projectile.png"
            );

            attackClock.restart();
        }
    }

    for (size_t i = 0; i < projectiles.size(); ++i) {
        projectiles[i].update(map);
    }
}

void MageEnemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);

    for (size_t i = 0; i < projectiles.size(); ++i) {
        projectiles[i].draw(window);
    }
}

void MageEnemy::takeDamage(int amount) {
    if (dead) return;

    health -= amount;

    flashing = true;
    flashClock.restart();
    sprite.setColor(sf::Color(255, 80, 80));

    if (health <= 0) {
        health = 0;
        dead = true;
        sprite.setTexture(deadTexture);
        sprite.setColor(sf::Color::White);

        sf::Vector2u size = deadTexture.getSize();

        if (size.x > 0 && size.y > 0) {
            sprite.setScale(32.f / size.x, 32.f / size.y);
        }
    }
}

bool MageEnemy::isDead() const {
    return dead;
}

sf::FloatRect MageEnemy::getBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f MageEnemy::getPosition() const {
    return sprite.getPosition();
}

std::vector<Projectile>& MageEnemy::getProjectiles() {
    return projectiles;
}