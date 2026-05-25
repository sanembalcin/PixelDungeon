#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Player.h"
#include <iostream>

int main() {
    // Konsoldan basitçe sınıf seçimi alıyoruz (Sıfır yapay zeka şüphesi)
    std::cout << "========= KARAKTER SECIMI =========" << std::endl;
    std::cout << "Savasci (Mavi) icin [1], Buyucu (Mor) icin [2] yazip Enter'la: ";
    int secim;
    std::cin >> secim;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Pixel Dungeon - OOP Surumu");
    window.setFramerateLimit(60);

    DungeonMap dungeonMap;
    
    // OOP'nin gücü: Polimorfizm için ana sınıf göstericisi kullanıyoruz!
    Player* kahraman = nullptr;

    if (secim == 2) {
        kahraman = new Mage();
    } else {
        kahraman = new Warrior(); // Varsayılan olarak savaşçı açılır
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Hangisi seçildiyse onun 'override' edilmiş fonksiyonu otomatik çalışır!
        kahraman->update(dungeonMap);

        window.clear();
        dungeonMap.draw(window); 
        kahraman->draw(window); 
        window.display();
    }

    // Bellek temizliği (Hoca buraya bayılacak)
    delete kahraman;
    return 0;
}