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
    // KİLİTLENMEYİ ÖNLEYEN EN KRİTİK DEĞİŞİKLİK: 
    // Saldırı yaparken bile WASD tuşları çalışmaya devam edecek, girdi engellenmeyecek!
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        sprite.move(0.f, -4.f);
        currentDir = Direction::UP;
        if (!isAttacking) sprite.setTexture(texUp);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        sprite.move(0.f, 4.f);
        currentDir = Direction::DOWN;
        if (!isAttacking) sprite.setTexture(texDown);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        sprite.move(-4.f, 0.f);
        currentDir = Direction::LEFT;
        if (!isAttacking) sprite.setTexture(texLeft);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        sprite.move(4.f, 0.f);
        currentDir = Direction::RIGHT;
        if (!isAttacking) sprite.setTexture(texRight);
    }

    // Space'e basınca eğer zaten saldırmıyorsak saldırıyı başlatıyoruz
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isAttacking) {
        isAttacking = true;
        attackClock.restart(); // Saati sıfırdan başlatıyoruz

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
    // Eğer karakter o an saldırıyorsa, zamanlayıcıyı sürekli kontrol et
    if (isAttacking) {
        // Belirlediğimiz süre (0.2 saniye) dolduysa saldırı animasyonunu bitir
        if (attackClock.getElapsedTime().asSeconds() >= attackDuration) {
            isAttacking = false;
            
            // Saldırı bittiği için karakteri durduğu yöndeki normal yürüme resmine geri döndür
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