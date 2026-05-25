#include "Player.h"
#include <iostream> // cout hatasını çözen sihirli kütüphane

Player::Player() {
    speed = 2.0f;
    hp = 100;
    attack = 10;
}

void Player::update(DungeonMap& map) {}
void Player::draw(sf::RenderWindow& window) {}

// ==========================================
// WARRIOR (Savaşçı) FONKSİYONLARI
// ==========================================
Warrior::Warrior() {
    speed = 2.5f;
    hp = 150;
    attack = 15;
    sprite.setPosition(60.0f, 60.0f);

    if (texture.loadFromFile("assets/warrior_sheet.png")) {
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(1, 0, 14, 16));
        sprite.setScale(2.0f, 2.0f);
    } else {
        std::cout << "Savasci spritesheet yuklenemedi!" << std::endl;
    }
}

void Warrior::update(DungeonMap& map) {
    sf::Vector2f nextPos = sprite.getPosition();
    bool hareketVar = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        nextPos.y -= speed;
        sprite.setTextureRect(sf::IntRect(16, 0, 16, 16)); 
        hareketVar = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        nextPos.y += speed;
        sprite.setTextureRect(sf::IntRect(0, 0, 16, 16)); 
        hareketVar = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        nextPos.x -= speed;
        sprite.setTextureRect(sf::IntRect(32, 0, 16, 16)); 
        hareketVar = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        nextPos.x += speed;
        sprite.setTextureRect(sf::IntRect(48, 0, 16, 16)); 
        hareketVar = true;
    }

    if (hareketVar) {
        int tileX = static_cast<int>((nextPos.x + 10) / TILE_SIZE);
        int tileY = static_cast<int>((nextPos.y + 10) / TILE_SIZE);

        if (map.grid[tileY][tileX] == 0) {
            sprite.setPosition(nextPos);
        }
    }
}

void Warrior::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// ==========================================
// MAGE (Büyücü) FONKSİYONLARI
// ==========================================
Mage::Mage() {
    speed = 3.5f;
    hp = 80;
    attack = 25;
    sprite.setPosition(520.0f, 60.0f);

    // 4 farklı yön resmini de hafızaya çekiyoruz
    texUp.loadFromFile("assets/mage_sheet.png");
    texDown.loadFromFile("assets/mage_sheet.png");
    texLeft.loadFromFile("assets/mage_sheet.png");
    texRight.loadFromFile("assets/mage_sheet.png");

    // İlk açılışta varsayılan görünüm (Aşağı bakan ilk kare)
    sprite.setTexture(texDown);
    sprite.setTextureRect(sf::IntRect(1, 0, 14, 16)); 
    sprite.setScale(2.0f, 2.0f);
}

void Mage::update(DungeonMap& map) {
    sf::Vector2f nextPos = sprite.getPosition();
    bool hareketVar = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        nextPos.y -= speed;
        sprite.setTexture(texUp);
        sprite.setTextureRect(sf::IntRect(16, 0, 16, 16)); // Yukarı bakan kare koordinatı
        hareketVar = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        nextPos.y += speed;
        sprite.setTexture(texDown);
        sprite.setTextureRect(sf::IntRect(0, 0, 16, 16)); // Aşağı bakan kare koordinatı
        hareketVar = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        nextPos.x -= speed;
        sprite.setTexture(texLeft);
        sprite.setTextureRect(sf::IntRect(32, 0, 16, 16)); // Sola bakan kare koordinatı
        hareketVar = true;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        nextPos.x += speed;
        sprite.setTexture(texRight);
        sprite.setTextureRect(sf::IntRect(48, 0, 16, 16)); // Sağa bakan kare koordinatı
        hareketVar = true;
    }

    if (hareketVar) {
        int tileX = static_cast<int>((nextPos.x + 10) / TILE_SIZE);
        int tileY = static_cast<int>((nextPos.y + 10) / TILE_SIZE);

        if (map.grid[tileY][tileX] == 0) {
            sprite.setPosition(nextPos);
        }
    }
}

void Mage::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}