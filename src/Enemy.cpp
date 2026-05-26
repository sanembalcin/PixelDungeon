#include "Enemy.h"
#include <cmath>
#include <cstdlib> // rand() fonksiyonu için

Enemy::Enemy(float startX, float startY) {
    // İnternetten bulduğun pikselli bir slime resmini assets'e "slime.png" olarak atabilirsin kanka
    if (!texture.loadFromFile("assets/slime.png")) {
        // Eğer resim henüz yoksa geçici olarak ne yaptığımızı görelim diye hata vermesin
    }
    
    sprite.setTexture(texture);
    sprite.setScale(0.06f, 0.06f); // Karakterinle aynı ölçekte başlasın
    sprite.setPosition(startX, startY);
    
    speed = 1.5f; // Oyuncudan biraz daha yavaş olsun ki kaçabilelim
    health = 3;   // Kılıçla 3 vuruşta ölsün
    isChasing = false;
    changeBehaviorTime = 2.0f; // 2 saniyede bir rastgele yön değiştirsin
    velocity = sf::Vector2f(0.f, 0.f);
}

void Enemy::update(const sf::Vector2f& playerPos) {
    sf::Vector2f enemyPos = sprite.getPosition();
    
    // 1. OYUNCU İLE ARADAKİ MESAFEYİ ÖLÇ (Hipotenüs)
    float dx = playerPos.x - enemyPos.x;
    float dy = playerPos.y - enemyPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    
    // 2. YAPAY ZEKA KARAR MEKANİZMASI
    if (distance < 150.f) {
        // Oyuncu 150 piksel yakındaysa: KOVALAMA MODU (Chase)
        isChasing = true;
        
        // Oyuncuya doğru giden yön vektörünü normalize ediyoruz
        sf::Vector2f direction(dx / distance, dy / distance);
        velocity = direction * speed;
    } 
    else {
        // Oyuncu uzaktaysa: DEVRİYE MODU (Patrol - Rastgele Gezinme)
        if (isChasing) {
            // Kovalamadan yeni çıkmışsa hızı bi sıfırla
            isChasing = false;
            behaviorClock.restart();
        }
        
        // 2 saniyede bir kafasına göre yön seçsin
        if (behaviorClock.getElapsedTime().asSeconds() >= changeBehaviorTime) {
            int randomDir = std::rand() % 5; // 0:Dur, 1:Yukarı, 2:Aşağı, 3:Sola, 4:Sağa
            
            if (randomDir == 0)      velocity = sf::Vector2f(0.f, 0.f);
            else if (randomDir == 1) velocity = sf::Vector2f(0.f, -speed);
            else if (randomDir == 2) velocity = sf::Vector2f(0.f, speed);
            else if (randomDir == 3) velocity = sf::Vector2f(-speed, 0.f);
            else if (randomDir == 4) velocity = sf::Vector2f(speed, 0.f);
            
            behaviorClock.restart();
        }
    }
    
    // Canavarı hareket ettir (İleride buraya harita duvar çarpışması da eklenecek)
    sprite.move(velocity);
}

void Enemy::takeDamage(int amount) {
    health -= amount;
}

void Enemy::draw(sf::RenderWindow& window) {
    // Slime'ı ekrana çiziyoruz, eğer resmi yoksa geçici olarak yeşil boyayabilirsin
    if (texture.getNativeHandle() == 0) {
        sprite.setColor(sf::Color::Green);
    }
    window.draw(sprite);
}