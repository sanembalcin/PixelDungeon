#include "Slime.h"
#include "Map.h"
#include <cmath>
#include <queue>

Slime::Slime(float startX, float startY) {
    if (!texture.loadFromFile("assets/slime.png")) {
    }

    sprite.setTexture(texture);
    sprite.setScale(0.06f, 0.06f);
    sprite.setPosition(startX, startY);

    speed = 1.8f;
    health = 100;
    damage = 15;
    dead = false;
    flashing = false;
}

std::vector<sf::Vector2i> Slime::findPath(const sf::Vector2i& start, const sf::Vector2i& target, const DungeonMap& map) {
    std::vector<sf::Vector2i> path;

    if (start == target) return path;

    std::queue<sf::Vector2i> q;

    std::vector<std::vector<bool>> visited(
        MAP_HEIGHT,
        std::vector<bool>(MAP_WIDTH, false)
    );

    std::vector<std::vector<sf::Vector2i>> parent(
        MAP_HEIGHT,
        std::vector<sf::Vector2i>(MAP_WIDTH, sf::Vector2i(-1, -1))
    );

    q.push(start);
    visited[start.y][start.x] = true;

    int dirX[] = { 0, 0, -1, 1 };
    int dirY[] = { -1, 1, 0, 0 };

    bool found = false;
    int searchCount = 0;

    while (!q.empty()) {
        searchCount++;

        if (searchCount > 1000) {
            break;
        }

        sf::Vector2i curr = q.front();
        q.pop();

        if (curr == target) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            int nextX = curr.x + dirX[i];
            int nextY = curr.y + dirY[i];

            if (nextX >= 0 &&
                nextX < MAP_WIDTH &&
                nextY >= 0 &&
                nextY < MAP_HEIGHT) {

                if (!visited[nextY][nextX] &&
                    map.grid[nextY][nextX] != 1) {

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
            path.push_back(curr);
            curr = parent[curr.y][curr.x];
        }
    }

    return path;
}

void Slime::update(const sf::Vector2f& playerPos, const DungeonMap& map) {
    static sf::Clock aiClock;

    if (flashing && flashClock.getElapsedTime().asSeconds() >= 0.15f) {
        sprite.setColor(sf::Color::White);
        flashing = false;
    }

    sf::Vector2f enemyPos = sprite.getPosition();

    sf::Vector2i enemyTile(
        static_cast<int>((enemyPos.x + 10.f) / TILE_SIZE),
        static_cast<int>((enemyPos.y + 10.f) / TILE_SIZE)
    );

    sf::Vector2i playerTile(
        static_cast<int>((playerPos.x + 10.f) / TILE_SIZE),
        static_cast<int>((playerPos.y + 10.f) / TILE_SIZE)
    );

    if (aiClock.getElapsedTime().asSeconds() >= 0.5f) {
        path = findPath(enemyTile, playerTile, map);
        aiClock.restart();
    }

    if (!path.empty()) {
        sf::Vector2i nextTile = path.back();

        sf::Vector2f targetPos(
            nextTile.x * TILE_SIZE + 4.f,
            nextTile.y * TILE_SIZE + 4.f
        );

        sf::Vector2f direction = targetPos - enemyPos;

        float dirLength = std::sqrt(
            direction.x * direction.x +
            direction.y * direction.y
        );

        if (dirLength > 2.f) {
            direction /= dirLength;
            sprite.move(direction * speed);
        }

        else {
            path.pop_back();
        }
    }
}

void Slime::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Slime::getBounds() const {
    return sprite.getGlobalBounds();
}

int Slime::getHealth() const {
    return health;
}

int Slime::getDamage() const {
    return damage;
}

sf::Vector2f Slime::getPosition() const {
    return sprite.getPosition();
}

void Slime::takeDamage(int amount) {
    health -= amount;

    flashing = true;
    flashClock.restart();

    sprite.setColor(sf::Color(255, 80, 80));

    if (health < 0)
        health = 0;
}

bool Slime::isDead() const {
    return health <= 0;
}

void Slime::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}