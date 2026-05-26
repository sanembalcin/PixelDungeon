#include "Player.h"
#include "Map.h"

Player::Player() {
    currentDir = Direction::DOWN;
}

Warrior::Warrior() : Player() {
    // 1. Normal Yürüyüş Resimlerini Yükle
    texUp.loadFromFile("assets/warrior_up.png");
    texDown.loadFromFile("assets/warrior_down.png");
    texLeft.loadFromFile("assets/warrior_left.png");
    texRight.loadFromFile("assets/warrior_right.png");

    // 2. Senin Attığın Yeni Saldırı Resimlerini Yükle
    atkUp.loadFromFile("assets/warrior_attack_up.png");
    atkDown.loadFromFile("assets/warrior_attack_down.png");
    atkLeft.loadFromFile("assets/warrior_attack_left.png");
    atkRight.loadFromFile("assets/warrior_attack_right.png");

    // İlk başlangıç ayarları
    sprite.setTexture(texDown);
    sprite.setScale(0.06f, 0.06f);
    sprite.setPosition(200.f, 250.f); // Güvenli, geniş koridor bölgesi
    
    isAttacking = false;
    attackDuration = 0.20f; // Saldırı efekti ekranda 0.2 saniye (200 milisaniye) kalacak
}

void Warrior::handleInput(const DungeonMap& map) {
    float moveSpeed = 4.f;
    
    // Karakterin mevcut konumunu ve sınırlarını alıyoruz
    sf::Vector2f pos = sprite.getPosition();
    
    // SFML sprite'ının genişlik ve yüksekliğini alıyoruz (Çarpışmayı tam köşelerden hesaplamak için)
    float playerWidth = sprite.getGlobalBounds().width;
    float playerHeight = sprite.getGlobalBounds().height;

    // 1. YUKARI HAREKET (W)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        currentDir = Direction::UP;
        if (!isAttacking) sprite.setTexture(texUp);
        
        // Karakterin bir sonraki adımda olacağı üst kenar pikselleri
        float nextY = pos.y - moveSpeed;
        
        // Pikselleri TILE_SIZE'a (40) bölerek matris indekslerini buluyoruz
        int tileTop = static_cast<int>(nextY / TILE_SIZE);
        int tileLeft = static_cast<int>(pos.x / TILE_SIZE);
        int tileRight = static_cast<int>((pos.x + playerWidth) / TILE_SIZE);
        
        // Eğer haritada gideceğimiz yerler duvar (1) değilse hareket et
        if (map.grid[tileTop][tileLeft] != 1 && map.grid[tileTop][tileRight] != 1) {
            sprite.move(0.f, -moveSpeed);
        }
    }
    // 2. AŞAĞI HAREKET (S)
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        currentDir = Direction::DOWN;
        if (!isAttacking) sprite.setTexture(texDown);
        
        float nextY = pos.y + playerHeight + moveSpeed;
        
        int tileBottom = static_cast<int>(nextY / TILE_SIZE);
        int tileLeft = static_cast<int>(pos.x / TILE_SIZE);
        int tileRight = static_cast<int>((pos.x + playerWidth) / TILE_SIZE);
        
        if (map.grid[tileBottom][tileLeft] != 1 && map.grid[tileBottom][tileRight] != 1) {
            sprite.move(0.f, moveSpeed);
        }
    }
    // 3. SOLA HAREKET (A)
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        currentDir = Direction::LEFT;
        if (!isAttacking) sprite.setTexture(texLeft);
        
        float nextX = pos.x - moveSpeed;
        
        int tileLeft = static_cast<int>(nextX / TILE_SIZE);
        int tileTop = static_cast<int>(pos.y / TILE_SIZE);
        int tileBottom = static_cast<int>((pos.y + playerHeight) / TILE_SIZE);
        
        if (map.grid[tileTop][tileLeft] != 1 && map.grid[tileBottom][tileLeft] != 1) {
            sprite.move(-moveSpeed, 0.f);
        }
    }
    // 4. SAĞA HAREKET (D)
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        currentDir = Direction::RIGHT;
        if (!isAttacking) sprite.setTexture(texRight);
        
        float nextX = pos.x + playerWidth + moveSpeed;
        
        int tileRight = static_cast<int>(nextX / TILE_SIZE);
        int tileTop = static_cast<int>(pos.y / TILE_SIZE);
        int tileBottom = static_cast<int>((pos.y + playerHeight) / TILE_SIZE);
        
        if (map.grid[tileTop][tileRight] != 1 && map.grid[tileBottom][tileRight] != 1) {
            sprite.move(moveSpeed, 0.f);
        }
    }

    // Space kontrolü (Saldırı tetikleyici)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isAttacking) {
        isAttacking = true;
        attackClock.restart(); // Saati sıfırdan başlatıyoruz (Karakter 0.2 saniye salgırıda kalacak)

        // Baktığı yöne göre senin o harika kılıç efektli resmini giydiriyoruz
        switch (currentDir) {
            case Direction::UP:    sprite.setTexture(atkUp); break;
            case Direction::DOWN:  sprite.setTexture(atkDown); break;
            case Direction::LEFT:  sprite.setTexture(atkLeft); break;
            case Direction::RIGHT: sprite.setTexture(atkRight); break;
        }
        // Yeni resim yüklenince boyutun bozulmaması için senin 0.06f ölçeğini sabitliyoruz
        sprite.setScale(0.06f, 0.06f);
    }
}

void Warrior::update(const DungeonMap& map) {
    if (isAttacking) {
        // 0.2 saniyelik saldırı süresi dolduysa normal yürüyüş spritelarına geri dön
        if (attackClock.getElapsedTime().asSeconds() >= attackDuration) {
            isAttacking = false;
            
            switch (currentDir) {
                case Direction::UP:    sprite.setTexture(texUp); break;
                case Direction::DOWN:  sprite.setTexture(texDown); break;
                case Direction::LEFT:  sprite.setTexture(texLeft); break;
                case Direction::RIGHT: sprite.setTexture(texRight); break;
            }
            sprite.setScale(0.06f, 0.06f);
        }
    }
}

void Warrior::draw(sf::RenderWindow& window) {
    // O yapay düz mavi çizgiyi (slashLine) tamamen tarihe gömdük kanka!
    // Karakteri kırmızıya boyamayı da kaldırdık, çünkü artık kendi orijinal saldırı resmi çiziliyor.
    window.draw(sprite);
}