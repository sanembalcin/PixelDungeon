#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <vector>

enum class ItemType {
    HEART,
    SPEED,
    SWORD,
    MIND
};

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Player {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
    int maxHealth;
    int health;
    sf::Clock damageClock;
    sf::Clock flashClock;
    float damageCooldown;
    bool damageFlash;
    std::vector<ItemType> inventory;
    int attackPower;
    float damageReduction;
    float viewRadius;

public:
    Player();
    virtual ~Player() = default;
    virtual void handleInput(const DungeonMap& map);
    virtual void update(const DungeonMap& map);
    virtual void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    void takeDamage(int amount);
    int getHealth() const;
    int getMaxHealth() const;
    void setHealth(int value);
    float getSpeed() const;
    void setMaxHealth(int value);
    void setAttackPower(int value);
    void setSpeed(float value);
    void setViewRadius(float value);
    void drawHealthBar(sf::RenderWindow& window);
    void move(float offsetX, float offsetY);
    virtual bool getIsAttacking() const;
    void setPosition(float x, float y);
    void heal(int amount);
    void boostAttack(int amount);
    void boostDefense(float amount);
    void addToInventory(ItemType type);
    int getItemCount(ItemType type) const;
    void setInventoryCounts(int heartCount, int speedCount, int swordCount, int mindCount);
    float getViewRadius() const;
    int getPotionCount() const;
    int getSwordCount() const;
    int getAttackPower() const;
    virtual void die();
};

class Warrior : public Player {
private:
    sf::Texture texUp;
    sf::Texture texDown;
    sf::Texture texLeft;
    sf::Texture texRight;
    sf::Texture atkUp;
    sf::Texture atkDown;
    sf::Texture atkLeft;
    sf::Texture atkRight;
    bool isAttacking;
    float attackDuration;
    sf::Clock attackClock;
    Direction currentDir;
    sf::Texture deadTexture;

public:
    Warrior();
    void handleInput(const DungeonMap& map) override;
    void update(const DungeonMap& map) override;
    void draw(sf::RenderWindow& window) override;
    bool getIsAttacking() const override;
    void die() override;
};

class Rogue : public Player {
private:
    sf::Texture texUp;
    sf::Texture texDown;
    sf::Texture texLeft;
    sf::Texture texRight;
    sf::Texture atkUp;
    sf::Texture atkDown;
    sf::Texture atkLeft;
    sf::Texture atkRight;
    sf::Texture deadTexture;
    bool isAttacking;
    float attackDuration;
    sf::Clock attackClock;
    Direction currentDir;
    bool isDashing;
    sf::Clock dashClock;
    float dashDuration;

public:
    Rogue();
    void handleInput(const DungeonMap& map) override;
    void update(const DungeonMap& map) override;
    void draw(sf::RenderWindow& window) override;
    bool getIsAttacking() const override;
    void die() override;
};

#endif