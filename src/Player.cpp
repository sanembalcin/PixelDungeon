#include "Player.h"
#include <string>

Player::Player() {
    speed = 3.f;
    maxHealth = 100;
    health = maxHealth;
    damageCooldown = 1.0f;
    damageFlash = false;
    attackPower = 15;
    damageReduction = 0.0f;
    viewRadius = 5.f * TILE_SIZE;
}

void Player::handleInput(const DungeonMap& map) {
}

void Player::update(const DungeonMap& map) {
}

void Player::die() {
}

void Player::draw(sf::RenderWindow& window) {
    if (damageFlash && flashClock.getElapsedTime().asSeconds() >= 0.15f) {
        sprite.setColor(sf::Color::White);
        damageFlash = false;
    }

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

        damageFlash = true;
        flashClock.restart();
        sprite.setColor(sf::Color(255, 80, 80));

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
    sf::Vector2f camPos = window.getView().getCenter();
    sf::Vector2f viewSize = window.getView().getSize();

    float left = camPos.x - viewSize.x / 2.f;
    float top = camPos.y - viewSize.y / 2.f;
    float barY = top + viewSize.y - 100.f;

    sf::RectangleShape panel(sf::Vector2f(viewSize.x, 100.f));
    panel.setFillColor(sf::Color(15, 15, 15, 230));
    panel.setPosition(left, barY);
    window.draw(panel);

    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        return;
    }

    float rowY = barY + 38.f;

    sf::RectangleShape hpBg(sf::Vector2f(120.f, 16.f));
    hpBg.setFillColor(sf::Color(60, 60, 60));
    hpBg.setPosition(left + 20.f, rowY);
    window.draw(hpBg);

    float hpRatio = static_cast<float>(health) / static_cast<float>(maxHealth);
    if (hpRatio < 0.f) hpRatio = 0.f;
    if (hpRatio > 1.f) hpRatio = 1.f;

    sf::RectangleShape hpBar(sf::Vector2f(120.f * hpRatio, 16.f));
    hpBar.setFillColor(sf::Color(220, 30, 30));
    hpBar.setPosition(left + 20.f, rowY);
    window.draw(hpBar);

    sf::Text hpText;
    hpText.setFont(font);
    hpText.setCharacterSize(14);
    hpText.setFillColor(sf::Color::White);
    hpText.setString("HP: " + std::to_string(health) + "/" + std::to_string(maxHealth));
    hpText.setPosition(left + 150.f, rowY - 2.f);
    window.draw(hpText);

    int hCount = 0;
    int spCount = 0;
    int sCount = 0;
    int mCount = 0;

    for (auto item : inventory) {
        if (item == ItemType::HEART) hCount++;
        if (item == ItemType::SPEED) spCount++;
        if (item == ItemType::SWORD) sCount++;
        if (item == ItemType::MIND) mCount++;
    }

    float startX = 280.f;
    float spacing = 100.f;
    float iconY = rowY - 4.f;
    float textY = rowY - 2.f;

    sf::Texture heartTex;
    heartTex.loadFromFile("assets/heart.png");
    sf::Sprite heartSprite;
    heartSprite.setTexture(heartTex);
    sf::Vector2u heartSize = heartTex.getSize();
    if (heartSize.x > 0 && heartSize.y > 0) {
        heartSprite.setScale(24.f / heartSize.x, 24.f / heartSize.y);
    }
    heartSprite.setPosition(left + startX, iconY);
    window.draw(heartSprite);

    sf::Text heartText;
    heartText.setFont(font);
    heartText.setCharacterSize(15);
    heartText.setFillColor(sf::Color::White);
    heartText.setString("x " + std::to_string(hCount));
    heartText.setPosition(left + startX + 34.f, textY);
    window.draw(heartText);

    sf::Texture speedTex;
    speedTex.loadFromFile("assets/speed.png");
    sf::Sprite speedSprite;
    speedSprite.setTexture(speedTex);
    sf::Vector2u speedSize = speedTex.getSize();
    if (speedSize.x > 0 && speedSize.y > 0) {
        speedSprite.setScale(24.f / speedSize.x, 24.f / speedSize.y);
    }
    speedSprite.setPosition(left + startX + spacing, iconY);
    window.draw(speedSprite);

    sf::Text speedText;
    speedText.setFont(font);
    speedText.setCharacterSize(15);
    speedText.setFillColor(sf::Color::White);
    speedText.setString("x " + std::to_string(spCount));
    speedText.setPosition(left + startX + spacing + 34.f, textY);
    window.draw(speedText);

    sf::Texture swordTex;
    swordTex.loadFromFile("assets/sword.png");
    sf::Sprite swordSprite;
    swordSprite.setTexture(swordTex);
    sf::Vector2u swordSize = swordTex.getSize();
    if (swordSize.x > 0 && swordSize.y > 0) {
        swordSprite.setScale(24.f / swordSize.x, 24.f / swordSize.y);
    }
    swordSprite.setPosition(left + startX + spacing * 2, iconY);
    window.draw(swordSprite);

    sf::Text swordText;
    swordText.setFont(font);
    swordText.setCharacterSize(15);
    swordText.setFillColor(sf::Color::White);
    swordText.setString("x " + std::to_string(sCount));
    swordText.setPosition(left + startX + spacing * 2 + 34.f, textY);
    window.draw(swordText);

    sf::Texture mindTex;
    mindTex.loadFromFile("assets/mind.png");
    sf::Sprite mindSprite;
    mindSprite.setTexture(mindTex);
    sf::Vector2u mindSize = mindTex.getSize();
    if (mindSize.x > 0 && mindSize.y > 0) {
        mindSprite.setScale(24.f / mindSize.x, 24.f / mindSize.y);
    }
    mindSprite.setPosition(left + startX + spacing * 3, iconY);
    window.draw(mindSprite);

    sf::Text mindText;
    mindText.setFont(font);
    mindText.setCharacterSize(15);
    mindText.setFillColor(sf::Color::White);
    mindText.setString("x " + std::to_string(mCount));
    mindText.setPosition(left + startX + spacing * 3 + 34.f, textY);
    window.draw(mindText);

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

    if (health > maxHealth)
        health = maxHealth;
}

void Player::boostAttack(int amount) {
    attackPower += amount;
}

float Player::getViewRadius() const {
    return viewRadius;
}

int Player::getAttackPower() const {
    return attackPower;
}

float Player::getSpeed() const {
    return speed;
}

void Player::setMaxHealth(int value) {
    maxHealth = value;
}

void Player::setAttackPower(int value) {
    attackPower = value;
}

void Player::setSpeed(float value) {
    speed = value;
}

void Player::setViewRadius(float value) {
    viewRadius = value;
}

void Player::boostDefense(float amount) {
    damageReduction += amount;

    if (damageReduction > 0.8f)
        damageReduction = 0.8f;
}

void Player::addToInventory(ItemType type) {
    inventory.push_back(type);

    if (type == ItemType::HEART) {
        if (health < maxHealth) {
        heal(10);
        }
        else {
            maxHealth += 10;
            health = maxHealth;
        }
    }
    else if (type == ItemType::SPEED) {
        speed += 0.5f;
    }
    else if (type == ItemType::SWORD) {
        boostAttack(5);
    }
    else if (type == ItemType::MIND) {
        viewRadius += 1.f * TILE_SIZE;
    }
}

int Player::getItemCount(ItemType type) const {
    int count = 0;

    for (auto item : inventory) {
        if (item == type) {
            count++;
        }
    }

    return count;
}

void Player::setInventoryCounts(int heartCount, int speedCount, int swordCount, int mindCount) {
    inventory.clear();

    for (int i = 0; i < heartCount; i++) {
        inventory.push_back(ItemType::HEART);
    }

    for (int i = 0; i < speedCount; i++) {
        inventory.push_back(ItemType::SPEED);
    }

    for (int i = 0; i < swordCount; i++) {
        inventory.push_back(ItemType::SWORD);
    }

    for (int i = 0; i < mindCount; i++) {
        inventory.push_back(ItemType::MIND);
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
    deadTexture.loadFromFile("assets/warrior_dead.png");

    sprite.setTexture(texDown);

    sf::Vector2u textureSize = texDown.getSize();

    if (textureSize.x > 0 && textureSize.y > 0) {
        sprite.setScale(32.f / textureSize.x, 32.f / textureSize.y);
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

        if (sprite.getPosition().y + sprite.getGlobalBounds().height > 600.f) {
            sprite.setPosition(oldPos);
            return;
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
    Player::draw(window);
}

bool Warrior::getIsAttacking() const {
    return isAttacking;
}

Rogue::Rogue() : Player() {
    texUp.loadFromFile("assets/rogue_up.png");
    texDown.loadFromFile("assets/rogue_down.png");
    texLeft.loadFromFile("assets/rogue_left.png");
    texRight.loadFromFile("assets/rogue_right.png");

    atkUp.loadFromFile("assets/rogue_attack_up.png");
    atkDown.loadFromFile("assets/rogue_attack_down.png");
    atkLeft.loadFromFile("assets/rogue_attack_left.png");
    atkRight.loadFromFile("assets/rogue_attack_right.png");

    deadTexture.loadFromFile("assets/rogue_dead.png");

    sprite.setTexture(texDown);

    sf::Vector2u textureSize = texDown.getSize();

    if (textureSize.x > 0 && textureSize.y > 0) {
        sprite.setScale(32.f / textureSize.x, 32.f / textureSize.y);
    }

    sprite.setPosition(100.f, 100.f);

    isAttacking = false;
    attackDuration = 0.12f;
    currentDir = Direction::DOWN;

    attackPower = 22;
    speed = 4.8f;
    maxHealth = 70;
    health = maxHealth;

    isDashing = false;
    dashDuration = 0.18f;
}

void Rogue::handleInput(const DungeonMap& map) {
    if (isAttacking) return;

    sf::Vector2f movement(0.f, 0.f);
    Direction prevDir = currentDir;

    float currentSpeed = speed;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
        currentSpeed = 9.f;
        isDashing = true;
        dashClock.restart();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        movement.y -= currentSpeed;
        currentDir = Direction::UP;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        movement.y += currentSpeed;
        currentDir = Direction::DOWN;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement.x -= currentSpeed;
        currentDir = Direction::LEFT;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movement.x += currentSpeed;
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

        if (sprite.getPosition().y + sprite.getGlobalBounds().height > 600.f) {
            sprite.setPosition(oldPos);
            return;
        }
    }
}

void Rogue::update(const DungeonMap& map) {
    if (isAttacking && attackClock.getElapsedTime().asSeconds() >= attackDuration) {
        isAttacking = false;

        if (currentDir == Direction::UP) sprite.setTexture(texUp);
        else if (currentDir == Direction::DOWN) sprite.setTexture(texDown);
        else if (currentDir == Direction::LEFT) sprite.setTexture(texLeft);
        else if (currentDir == Direction::RIGHT) sprite.setTexture(texRight);
    }

    if (isDashing && dashClock.getElapsedTime().asSeconds() >= dashDuration) {
        isDashing = false;
    }
}

void Rogue::draw(sf::RenderWindow& window) {
    Player::draw(window);
}

bool Rogue::getIsAttacking() const {
    return isAttacking;
}

void Warrior::die() {

    sprite.setTexture(deadTexture);

    sf::Vector2u size = deadTexture.getSize();

    if (size.x > 0 && size.y > 0) {

        sprite.setScale(
            32.f / size.x,
            32.f / size.y
        );
    }
}

void Rogue::die() {

    sprite.setTexture(deadTexture);

    sf::Vector2u size = deadTexture.getSize();

    if (size.x > 0 && size.y > 0) {

        sprite.setScale(
            32.f / size.x,
            32.f / size.y
        );
    }
}