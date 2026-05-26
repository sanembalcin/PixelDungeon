#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Player {
protected:
    sf::Sprite sprite;
    sf::Texture texUp, texDown, texLeft, texRight;
    Direction currentDir;

public:
    Player();
    virtual ~Player() = default;
    virtual void handleInput(const class DungeonMap& map) = 0; 
    virtual void update(const class DungeonMap& map) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
};

class Warrior : public Player {
private:
    
    sf::Texture atkUp, atkDown, atkLeft, atkRight;
    
    bool isAttacking;
    sf::Clock attackClock;       
    float attackDuration;        

public:
    Warrior();
    void handleInput(const class DungeonMap& map) override;
    void update(const class DungeonMap& map) override;
    void draw(sf::RenderWindow& window) override;
};

#endif