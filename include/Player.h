#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Map.h"

class Player {
protected:
    sf::Sprite sprite;
    sf::Texture texture; // Derleyicinin aradığı ana texture değişkeni burada!
    float speed;
    int hp;
    int attack;

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

class Mage : public Player {
private:
    // Büyücüye özel 4 yön dokusu
    sf::Texture texUp;
    sf::Texture texDown;
    sf::Texture texLeft;
    sf::Texture texRight;

public:
    Mage();
    void update(DungeonMap& map) override;
    void draw(sf::RenderWindow& window) override;
};

#endif