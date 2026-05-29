#include "Skeleton.h"
#include <cmath>
#include <queue>

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
    health = 100;
    damage = 15;
    dead = false;
    attacking = false;
    flashing = false;
    attackCooldown = 1.0f;
    direction = 1;
}

std::vector<sf::Vector2i> Skeleton::findPath(const sf::Vector2i& start, const sf::Vector2i& target, const DungeonMap& map) {
    std::vector<sf::Vector2i> result;

    if (start == target) return result;

    if (start.x < 0 || start.x >= MAP_WIDTH || start.y < 0 || start.y >= MAP_HEIGHT) return result;
    if (target.x < 0 || target.x >= MAP_WIDTH || target.y < 0 || target.y >= MAP_HEIGHT) return result;

    std::queue<sf::Vector2i> q;
    std::vector<std::vector<bool>> visited(MAP_HEIGHT, std::vector<bool>(MAP_WIDTH, false));
    std::vector<std::vector<sf::Vector2i>> parent(MAP_HEIGHT, std::vector<sf::Vector2i>(MAP_WIDTH, sf::Vector2i(-1, -1)));

    q.push(start);
    visited[start.y][start.x] = true;

    int dirX[] = { 0, 0, -1, 1 };
    int dirY[] = { -1, 1, 0, 0 };

    bool found = false;

    while (!q.empty()) {
        sf::Vector2i curr = q.front();
        q.pop();

        if (curr == target) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            int nextX = curr.x + dirX[i];
            int nextY = curr.y + dirY[i];

            if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT) {
                if (!visited[nextY][nextX] && map.grid[nextY][nextX] != 1) {
                    visited[nextY][nextX] = true;
                    parent[nextY][nextX] = curr;
                    q.push(sf::Vector2i(nextX, nextY));
                }
            }
        }
    }

    if (found) {
        sf::Vector2i curr = target;

        while (curr != start) {
            result.push_back(curr);
            curr = parent[curr.y][curr.x];
        }
    }

    return result;
}

void Skeleton::update(const sf::Vector2f& playerPos, const DungeonMap& map) {
    if (dead) return;

    if (flashing && flashClock.getElapsedTime().asSeconds() >= 0.15f) {
        sprite.setColor(sf::Color::White);
        flashing = false;
    }

    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f diff = playerPos - pos;

    float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (distance < 45.f) {
        attacking = true;

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

    if (distance < 320.f) {
        sf::Vector2i enemyTile(
            static_cast<int>((pos.x + 16.f) / TILE_SIZE),
            static_cast<int>((pos.y + 16.f) / TILE_SIZE)
        );

        sf::Vector2i playerTile(
            static_cast<int>((playerPos.x + 16.f) / TILE_SIZE),
            static_cast<int>((playerPos.y + 16.f) / TILE_SIZE)
        );

        if (aiClock.getElapsedTime().asSeconds() >= 0.25f) {
            path = findPath(enemyTile, playerTile, map);
            aiClock.restart();
        }

        if (!path.empty()) {
            sf::Vector2i nextTile = path.back();
            sf::Vector2f targetPos(nextTile.x * TILE_SIZE, nextTile.y * TILE_SIZE);

            sf::Vector2f moveDir = targetPos - pos;
            float length = std::sqrt(moveDir.x * moveDir.x + moveDir.y * moveDir.y);

            if (length > 2.f) {
                moveDir /= length;
                sprite.move(moveDir * speed);

                if (std::abs(moveDir.x) > std::abs(moveDir.y)) {
                    if (moveDir.x > 0) {
                        sprite.setTexture(texRight);
                        direction = 3;
                    }
                    else {
                        sprite.setTexture(texLeft);
                        direction = 2;
                    }
                }
                else {
                    if (moveDir.y > 0) {
                        sprite.setTexture(texDown);
                        direction = 1;
                    }
                    else {
                        sprite.setTexture(texUp);
                        direction = 0;
                    }
                }
            }
            else {
                path.pop_back();
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
    flashing = true;
    flashClock.restart();
    sprite.setColor(sf::Color(255, 90, 90));

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