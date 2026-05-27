#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <vector>

enum class ItemType {
    POTION,
    SWORD,
    ARMOR
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
    float damageCooldown;
    std::vector<ItemType> inventory;
    int attackPower;
    float damageReduction;

public:
    Player();
    void handleInput();
    void update(const class DungeonMap& map);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    void takeDamage(int amount);
    int getHealth() const;
    int getMaxHealth() const;
    void setHealth(int value);
    void drawHealthBar(sf::RenderWindow& window);
    void move(float offsetX, float offsetY);
    virtual bool getIsAttacking() const;
    void setPosition(float x, float y);
    void heal(int amount);
    void boostAttack(int amount);
    void boostDefense(float amount);
    void addToInventory(ItemType type);
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

public:
    Warrior();
    void handleInput(const DungeonMap& map);
    void update(const DungeonMap& map);
    void draw(sf::RenderWindow& window);
    bool getIsAttacking() const override;
};

#endif