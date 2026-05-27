#include "Player.h"
#include <string>

Player::Player() {
    speed = 3.f;
    maxHealth = 100;
    health = maxHealth;
    damageCooldown = 1.0f;
    attackPower = 10;
    damageReduction = 0.0f;
}

void Player::handleInput() {
}

void Player::update(const DungeonMap& map) {
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
    if (damageClock.getElapsedTime().asSeconds() >= damageCooldown) {
        int finalDamage = amount - static_cast<int>(amount * damageReduction);
        if (finalDamage < 1) finalDamage = 1;
        health -= finalDamage;
        if (health < 0) health = 0;
        damageClock.restart();
    }
}

int Player::getHealth() const {
    return health;
}

int Player::getMaxHealth() const {
    return maxHealth;
}

void Player::setHealth(int value) {
    health = value;

    if (health > maxHealth)
        health = maxHealth;

    if (health < 0)
        health = 0;
}

void Player::drawHealthBar(sf::RenderWindow& window) {
    sf::RectangleShape bgBar(sf::Vector2f(100.f, 10.f));
    bgBar.setFillColor(sf::Color(50, 50, 50));

    sf::RectangleShape fgBar(sf::Vector2f(100.f * (static_cast<float>(health) / maxHealth), 10.f));
    fgBar.setFillColor(sf::Color::Red);

    sf::Vector2f camPos = window.getView().getCenter();
    sf::Vector2f viewSize = window.getView().getSize();

    sf::Vector2f uiPos(camPos.x - viewSize.x / 2.f + 10.f, camPos.y - viewSize.y / 2.f + 10.f);
    bgBar.setPosition(uiPos);
    fgBar.setPosition(uiPos);

    window.draw(bgBar);
    window.draw(fgBar);

    sf::Font font;
    if (font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        sf::Text uiText;
        uiText.setFont(font);
        uiText.setCharacterSize(12);
        uiText.setFillColor(sf::Color::White);

        int pCount = 0;
        int sCount = 0;
        int aCount = 0;

        for (auto item : inventory) {
            if (item == ItemType::POTION) pCount++;
            if (item == ItemType::SWORD) sCount++;
            if (item == ItemType::ARMOR) aCount++;
        }

        std::string info = "ATK: " + std::to_string(attackPower) + " | Iksir: " + std::to_string(pCount) + " Kilic: " + std::to_string(sCount) + " Zirh: " + std::to_string(aCount);
        uiText.setString(info);
        uiText.setPosition(uiPos.x, uiPos.y + 15.f);
        window.draw(uiText);
    }
}

void Player::move(float offsetX, float offsetY) {
    sprite.move(offsetX, offsetY);
}

bool Player::getIsAttacking() const {
    return false;
}

void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Player::heal(int amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
}

void Player::boostAttack(int amount) {
    attackPower += amount;
}

void Player::boostDefense(float amount) {
    damageReduction += amount;
    if (damageReduction > 0.8f) damageReduction = 0.8f;
}

void Player::addToInventory(ItemType type) {
    inventory.push_back(type);

    if (type == ItemType::POTION) {
        heal(20);
    }
    else if (type == ItemType::SWORD) {
        boostAttack(5);
    }
    else if (type == ItemType::ARMOR) {
        boostDefense(0.1f);
    }
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

    sf::Vector2u textureSize = texDown.getSize();
    if (textureSize.x > 0 && textureSize.y > 0) {
        float scaleX = 32.f / textureSize.x;
        float scaleY = 32.f / textureSize.y;
        sprite.setScale(scaleX, scaleY);
    }

    sprite.setPosition(100.f, 100.f);

    isAttacking = false;
    attackDuration = 0.2f;
    currentDir = Direction::DOWN;

    attackPower = 15;
    speed = 3.5f;
}

void Warrior::handleInput(const DungeonMap& map) {
    if (isAttacking) return;

    sf::Vector2f movement(0.f, 0.f);
    Direction prevDir = currentDir;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        movement.y -= speed;
        currentDir = Direction::UP;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        movement.y += speed;
        currentDir = Direction::DOWN;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement.x -= speed;
        currentDir = Direction::LEFT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movement.x += speed;
        currentDir = Direction::RIGHT;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        isAttacking = true;
        attackClock.restart();

        if (currentDir == Direction::UP) sprite.setTexture(atkUp);
        else if (currentDir == Direction::DOWN) sprite.setTexture(atkDown);
        else if (currentDir == Direction::LEFT) sprite.setTexture(atkLeft);
        else if (currentDir == Direction::RIGHT) sprite.setTexture(atkRight);

        return;
    }

    if (movement.x != 0.f || movement.y != 0.f) {
        if (currentDir != prevDir) {
            if (currentDir == Direction::UP) sprite.setTexture(texUp);
            else if (currentDir == Direction::DOWN) sprite.setTexture(texDown);
            else if (currentDir == Direction::LEFT) sprite.setTexture(texLeft);
            else if (currentDir == Direction::RIGHT) sprite.setTexture(texRight);
        }

        sf::Vector2f oldPos = sprite.getPosition();
        sprite.move(movement);

        sf::Vector2f pos = sprite.getPosition();

        float hitboxLeft = pos.x + 10.f;
        float hitboxTop = pos.y + 14.f;
        float hitboxWidth = 12.f;
        float hitboxHeight = 14.f;

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

void Warrior::update(const DungeonMap& map) {
    if (isAttacking && attackClock.getElapsedTime().asSeconds() >= attackDuration) {
        isAttacking = false;

        if (currentDir == Direction::UP) sprite.setTexture(texUp);
        else if (currentDir == Direction::DOWN) sprite.setTexture(texDown);
        else if (currentDir == Direction::LEFT) sprite.setTexture(texLeft);
        else if (currentDir == Direction::RIGHT) sprite.setTexture(texRight);
    }
}

void Warrior::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool Warrior::getIsAttacking() const {
    return isAttacking;
}