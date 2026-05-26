#include "Player.h"
#include <iostream>

Player::Player() {
    speed = 2.0f;
    hp = 100;
    attack = 10;
    isAttacking = false;
    lastDirection = "S"; 
}

void Player::update(DungeonMap& map) {}
void Player::draw(sf::RenderWindow& window) {}

// ==========================================
// WARRIOR (Savaşçı Sınıfı Fonksiyonları)
// ==========================================
Warrior::Warrior() {
    speed = 2.5f;
    hp = 150;
    attack = 15;
    isAttacking = false;
    lastDirection = "S"; 
    sprite.setPosition(60.0f, 60.0f);

    if (texture.loadFromFile("assets/warrior_down.png")) {
        sprite.setTexture(texture);
        sprite.setScale(0.06f, 0.06f); 
    } else {
        std::cout << "[HATA] assets/warrior_down.png yuklenemedi!" << std::endl;
    }
}

void Warrior::update(DungeonMap& map) {
    // Saldırı yapılıyorsa yön tuşlarını kilitle kanka
    if (isAttacking) {
        if (attackClock.getElapsedTime().asSeconds() > 0.12f) {
            isAttacking = false;
            sprite.setColor(sf::Color::White); // Rengi normale döndür
        }
        return; 
    }

    sf::Vector2f nextPos = sprite.getPosition();
    bool hareketVar = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        nextPos.y -= speed;
        if (texture.loadFromFile("assets/warrior_up.png")) sprite.setTexture(texture);
        lastDirection = "W";
        hareketVar = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        nextPos.y += speed;
        if (texture.loadFromFile("assets/warrior_down.png")) sprite.setTexture(texture);
        lastDirection = "S";
        hareketVar = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        nextPos.x -= speed;
        if (texture.loadFromFile("assets/warrior_left.png")) sprite.setTexture(texture);
        lastDirection = "A";
        hareketVar = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        nextPos.x += speed;
        if (texture.loadFromFile("assets/warrior_right.png")) sprite.setTexture(texture);
        lastDirection = "D";
        hareketVar = true;
    }

    // Space tuşuna basınca saldırıyı başlat kanka
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        isAttacking = true;
        attackClock.restart();
        sprite.setColor(sf::Color(255, 100, 100, 230)); // Vuruş hissi için kırmızı yap
    }

    if (hareketVar) {
        float width = sprite.getGlobalBounds().width;
        float height = sprite.getGlobalBounds().height;

        // 🎯 Ofset sayılarını (6 ve 10 gibi) büyüterek kontrol kutusunu karakterin merkezine yaklaştırdık.
        // Böylece karakter dar koridorlardan geçerken pürüzsüzce kayacak kanka.
        int leftTile   = static_cast<int>((nextPos.x + 6) / TILE_SIZE);
        int rightTile  = static_cast<int>((nextPos.x + width - 6) / TILE_SIZE);
        int topTile    = static_cast<int>((nextPos.y + 10) / TILE_SIZE);
        int bottomTile = static_cast<int>((nextPos.y + height - 2) / TILE_SIZE);

        bool collision = false;

        if (leftTile < 0 || rightTile >= MAP_WIDTH || topTile < 0 || bottomTile >= MAP_HEIGHT) {
            collision = true;
        } else {
            if (map.grid[topTile][leftTile] == 1 ||
                map.grid[topTile][rightTile] == 1 ||
                map.grid[bottomTile][leftTile] == 1 ||
                map.grid[bottomTile][rightTile] == 1) {
                collision = true;
            }
        }

        if (!collision) {
            sprite.setPosition(nextPos);
        }
    }
}

void Warrior::draw(sf::RenderWindow& window) {
    window.draw(sprite);

    // Eğer saldırı aktifse çizgiyi ekrana bas kanka
    if (isAttacking) {
        sf::Vector2f pPos = sprite.getPosition();
        // Karakterimizin tam merkez noktasını buluyoruz
        float cX = pPos.x + 15.0f;
        float cY = pPos.y + 15.0f;

        // Çizgiyi 25 piksel yerine 18 piksel yaparak biraz kısalttık kanka
        sf::RectangleShape slashLine(sf::Vector2f(18.0f, 4.0f));
        slashLine.setFillColor(sf::Color(200, 230, 255, 200)); 
        slashLine.setOrigin(0.0f, 2.0f); 

        // 🎯 Düzelttik: Ofsetleri 15'ten 5'e indirdik. Çizgi artık duvara taşmayacak!
        if (lastDirection == "W") {
            slashLine.setPosition(cX, cY - 5.0f);
            slashLine.setRotation(270.0f);
        }
        else if (lastDirection == "S") {
            slashLine.setPosition(cX, cY + 5.0f);
            slashLine.setRotation(90.0f);
        }
        else if (lastDirection == "A") {
            slashLine.setPosition(cX - 5.0f, cY);
            slashLine.setRotation(180.0f);
        }
        else if (lastDirection == "D") {
            slashLine.setPosition(cX + 5.0f, cY);
            slashLine.setRotation(0.0f);
        }

        window.draw(slashLine);
    }
}