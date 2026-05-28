#include "Skeleton.h"
#include <cmath>

Skeleton::Skeleton(float x, float y) {
    texUp.loadFromFile("assets/skeleton_up.png");
    texDown.loadFromFile("assets/skeleton_down.png");
    texLeft.loadFromFile("assets/skeleton_left.png");
    texRight.loadFromFile("assets/skeleton_right.png");

    atkUp.loadFromFile("assets/skeleton_attack_up.png");
    atkDown.loadFromFile("assets/skeleton_attack_down.png");
    atkLeft.loadFromFile("assets/skeleton_attack_left.png");
    atkRight.loadFromFile("assets/skeleton_attack_right.png");

    deadTexture.loadFromFile("assets/skeleton_dead.png");

    sprite.setTexture(texDown);

    sf::Vector2u size = texDown.getSize();
    if (size.x > 0 && size.y > 0) {
        sprite.setScale(32.f / size.x, 32.f / size.y);
    }

    sprite.setPosition(x, y);

    speed = 2.4f;
    health = 120;
    damage = 25;
    dead = false;
    attacking = false;
    attackCooldown = 1.0f;
    direction = 1;
}

void Skeleton::update(const sf::Vector2f& playerPos, const DungeonMap& map) {
    if (dead) return;

    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f diff = playerPos - pos;

    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (distance < 50.f) {
        attacking = true;
        sprite.setColor(sf::Color(255, 120, 120));

        if (std::abs(diff.x) > std::abs(diff.y)) {
            if (diff.x > 0) {
                sprite.setTexture(atkRight);
                direction = 3;
            }
            else {
                sprite.setTexture(atkLeft);
                direction = 2;
            }
        }
        else {
            if (diff.y > 0) {
                sprite.setTexture(atkDown);
                direction = 1;
            }
            else {
                sprite.setTexture(atkUp);
                direction = 0;
            }
        }

        return;
    }

    attacking = false;

    if (distance < 320.f && distance > 1.f) {
        diff /= distance;

        sf::Vector2f oldPos = sprite.getPosition();
        sprite.move(diff * speed);

        if (std::abs(diff.x) > std::abs(diff.y)) {
            if (diff.x > 0) {
                sprite.setTexture(texRight);
                direction = 3;
            }
            else {
                sprite.setTexture(texLeft);
                direction = 2;
            }
        }
        else {
            if (diff.y > 0) {
                sprite.setTexture(texDown);
                direction = 1;
            }
            else {
                sprite.setTexture(texUp);
                direction = 0;
            }
        }

        sf::Vector2f newPos = sprite.getPosition();

        float hitboxLeft = newPos.x + 10.f;
        float hitboxTop = newPos.y + 12.f;
        float hitboxWidth = 12.f;
        float hitboxHeight = 16.f;

        int leftTile = static_cast<int>(hitboxLeft / TILE_SIZE);
        int rightTile = static_cast<int>((hitboxLeft + hitboxWidth) / TILE_SIZE);
        int topTile = static_cast<int>(hitboxTop / TILE_SIZE);
        int bottomTile = static_cast<int>((hitboxTop + hitboxHeight) / TILE_SIZE);

        if (leftTile < 0 || rightTile >= MAP_WIDTH || topTile < 0 || bottomTile >= MAP_HEIGHT) {
            sprite.setPosition(oldPos);
            return;
        }

        for (int y = topTile; y <= bottomTile; ++y) {
            for (int x = leftTile; x <= rightTile; ++x) {
                if (map.grid[y][x] == 1) {
                    sprite.setPosition(oldPos);
                    return;
                }
            }
        }
    }
}

void Skeleton::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2f Skeleton::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect Skeleton::getBounds() const {
    return sprite.getGlobalBounds();
}

void Skeleton::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Skeleton::takeDamage(int amount) {
    if (dead) return;

    health -= amount;

    if (health <= 0) {
        health = 0;
        dead = true;
        sprite.setTexture(deadTexture);

        sf::Vector2u size = deadTexture.getSize();
        if (size.x > 0 && size.y > 0) {
            sprite.setScale(32.f / size.x, 32.f / size.y);
        }
    }
}

bool Skeleton::isDead() const {
    return dead;
}

int Skeleton::getDamage() const {
    return damage;
}

bool Skeleton::canAttack() const {
    return attacking;
}

bool Skeleton::tryAttack() {
    if (attacking && attackClock.getElapsedTime().asSeconds() >= attackCooldown) {
        attackClock.restart();
        return true;
    }

    return false;
}