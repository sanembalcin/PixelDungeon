#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include "Player.h" // Oyuncunun konumunu alabilmek için

class Enemy {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    
    sf::Vector2f velocity;
    float speed;
    int health;
    
    // Yapay zeka zamanlayıcıları
    sf::Clock behaviorClock;
    float changeBehaviorTime;
    bool isChasing;

public:
    Enemy(float startX, float startY);
    void update(const sf::Vector2f& playerPos);
    void draw(sf::RenderWindow& window);
    
    // İleride kılıçla vurabilmek için sınır kutusu (hitbox) ve can fonksiyonları
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    void takeDamage(int amount);
    int getHealth() const { return health; }
};

#endif