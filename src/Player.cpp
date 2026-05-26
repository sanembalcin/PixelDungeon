#include "Player.h"
#include "Map.h"
#include <iostream>

Player::Player() {
    if (!texture.loadFromFile("assets/warrior_down.png")) {
    }
    sprite.setTexture(texture);
    sprite.setScale(0.06f, 0.06f);
    sprite.setPosition(100.f, 100.f);
    speed = 3.0f;
    maxHealth = 100;
    health = maxHealth;
    damageCooldown = 1.0f;
}

void Player::handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) sprite.move(0, -speed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) sprite.move(0, speed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) sprite.move(-speed, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) sprite.move(speed, 0);
}

void Player::update(const DungeonMap& map) {
    handleInput();
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

void Player::takeDamage(int amount) {
    if (damageClock.getElapsedTime().asSeconds() < damageCooldown) {
        return; 
    }
    health -= amount;
    if (health < 0) health = 0;
    damageClock.restart();
}

int Player::getHealth() const { 
    return health; 
}

int Player::getMaxHealth() const { 
    return maxHealth; 
}

Warrior::Warrior() : Player() {
    texUp.loadFromFile("assets/warrior_up.png");
    texDown.loadFromFile("assets/warrior_down.png");
    texLeft.loadFromFile("assets/warrior_left.png");
    texRight.loadFromFile("assets/warrior_right.png");
    atkUp.loadFromFile("assets/warrior_attack_up.png");
    atkDown.loadFromFile("assets/warrior_attack_down.png");
    atkLeft.loadFromFile("assets/warrior_attack_left.png");
    atkRight.loadFromFile("assets/warrior_attack_right.png");
    sprite.setTexture(texDown);
    sprite.setScale(0.06f, 0.06f);
    sprite.setPosition(200.f, 250.f); 
    isAttacking = false;
    attackDuration = 0.20f; 
}

void Warrior::handleInput(const DungeonMap& map) {
    float moveSpeed = 4.f;
    sf::Vector2f pos = sprite.getPosition();
    
    float boxW = 10.f;
    float boxH = 10.f;
    float offsetX = 11.f;
    float offsetY = 16.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        currentDir = Direction::UP;
        if (!isAttacking) sprite.setTexture(texUp);
        float nextY = pos.y - moveSpeed;
        if (nextY >= 0.f) {
            int tileTop = static_cast<int>((nextY + offsetY) / TILE_SIZE);
            int tileLeft = static_cast<int>((pos.x + offsetX) / TILE_SIZE);
            int tileRight = static_cast<int>((pos.x + offsetX + boxW) / TILE_SIZE);
            if (map.grid[tileTop][tileLeft] != 1 && map.grid[tileTop][tileRight] != 1) {
                
                sprite.move(0.f, -moveSpeed);
            }
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        currentDir = Direction::DOWN;
        if (!isAttacking) sprite.setTexture(texDown);
        float nextY = pos.y + moveSpeed;
        if (nextY < MAP_HEIGHT * TILE_SIZE) {
            int tileBottom = static_cast<int>((nextY + offsetY + boxH) / TILE_SIZE);
            int tileLeft = static_cast<int>((pos.x + offsetX) / TILE_SIZE);
            int tileRight = static_cast<int>((pos.x + offsetX + boxW) / TILE_SIZE);
            if (map.grid[tileBottom][tileLeft] != 1 && map.grid[tileBottom][tileRight] != 1) {
                sprite.move(0.f, moveSpeed);
            }
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        currentDir = Direction::LEFT;
        if (!isAttacking) sprite.setTexture(texLeft);
        float nextX = pos.x - moveSpeed;
        if (nextX >= 0.f) {
            int tileLeft = static_cast<int>((nextX + offsetX) / TILE_SIZE);
            int tileTop = static_cast<int>((pos.y + offsetY) / TILE_SIZE);
            int tileBottom = static_cast<int>((pos.y + offsetY + boxH) / TILE_SIZE);
            if (map.grid[tileTop][tileLeft] != 1 && map.grid[tileBottom][tileLeft] != 1) {
                sprite.move(-moveSpeed, 0.f);
            }
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        currentDir = Direction::RIGHT;
        if (!isAttacking) sprite.setTexture(texRight);
        float nextX = pos.x + moveSpeed;
        if (nextX < MAP_WIDTH * TILE_SIZE) {
            int tileRight = static_cast<int>((nextX + offsetX + boxW) / TILE_SIZE);
            int tileTop = static_cast<int>((pos.y + offsetY) / TILE_SIZE);
            int tileBottom = static_cast<int>((pos.y + offsetY + boxH) / TILE_SIZE);
            if (map.grid[tileTop][tileRight] != 1 && map.grid[tileBottom][tileRight] != 1) {
                sprite.move(moveSpeed, 0.f);
            }
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isAttacking) {
        isAttacking = true;
        attackClock.restart(); 
        switch (currentDir) {
            case Direction::UP:    sprite.setTexture(atkUp); break;
            case Direction::DOWN:  sprite.setTexture(atkDown); break;
            case Direction::LEFT:  sprite.setTexture(atkLeft); break;
            case Direction::RIGHT: sprite.setTexture(atkRight); break;
        }
        sprite.setScale(0.06f, 0.06f);
    }
}

void Warrior::update(const DungeonMap& map) {
    handleInput(map);

    if (isAttacking) {
        if (attackClock.getElapsedTime().asSeconds() >= attackDuration) {
            isAttacking = false;
            switch (currentDir) {
            case Direction::UP: sprite.setTexture(texUp); break;
            case Direction::DOWN: sprite.setTexture(texDown); break;
            case Direction::LEFT: sprite.setTexture(texLeft); break;
            case Direction::RIGHT: sprite.setTexture(texRight); break;
            }
            sprite.setScale(0.06f, 0.06f);
        }
    }

    sf::Vector2f pos = sprite.getPosition();
    float w = sprite.getGlobalBounds().width;
    float h = sprite.getGlobalBounds().height;

    float minX = 0.f;
    float minY = 0.f;
    float maxX = static_cast<float>(MAP_WIDTH * TILE_SIZE) - w;
    float maxY = static_cast<float>(MAP_HEIGHT * TILE_SIZE) - h;

    if (pos.x < minX) pos.x = minX;
    if (pos.x > maxX) pos.x = maxX;
    if (pos.y < minY) pos.y = minY;
    if (pos.y > maxY) pos.y = maxY;

    sprite.setPosition(pos);
}

void Warrior::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Player::drawHealthBar(sf::RenderWindow& window) {
    sf::RectangleShape backBar;
    backBar.setSize(sf::Vector2f(200.f, 20.f));
    backBar.setFillColor(sf::Color(50, 50, 50));
    backBar.setPosition(20.f, 20.f);

    sf::RectangleShape frontBar;
    float healthRatio = static_cast<float>(health) / maxHealth;
    frontBar.setSize(sf::Vector2f(200.f * healthRatio, 20.f));
    frontBar.setFillColor(sf::Color(200, 0, 0));
    frontBar.setPosition(20.f, 20.f);

    window.draw(backBar);
    window.draw(frontBar);
}

void Player::move(float offsetX, float offsetY) {
    sprite.move(offsetX, offsetY);
}

bool Player::getIsAttacking() const {
    return false;
}

bool Warrior::getIsAttacking() const {
    return isAttacking;
}

void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}