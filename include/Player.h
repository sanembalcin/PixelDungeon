#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Map.h"
#include <string>

class Player {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
    int hp;
    int attack;

    // ⚔️ DERLEYİCİNİN BULAMADIĞI SİHİRLİ DEĞİŞKENLER BURADA OLMAK ZORUNDA:
    bool isAttacking;
    sf::Clock attackClock;       
    std::string lastDirection;   

public:
    Player();
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    int getHp() const { return hp; }

    virtual void update(DungeonMap& map);
    virtual void draw(sf::RenderWindow& window);
};

class Warrior : public Player {
public:
    Warrior();
    void update(DungeonMap& map) override;
    void draw(sf::RenderWindow& window) override;
};

#endif