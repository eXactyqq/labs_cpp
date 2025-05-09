#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    void displayInfo() const {
        std::cout << name << " - HP: " << health << ", Attack: " << attack << ", Defense: " << defense << "\n";
    }

    void takeDamage(int damage) {
        int netDamage = damage - defense;
        if (netDamage < 0) netDamage = 0;
        health -= netDamage;
        if (health < 0) {
            health = 0;
        }
    }

    int getAttack() const { return attack; }
    bool isAlive() const { return health > 0; }
    std::string getName() const { return name; }
    int getHealth() const { return health; }
};

std::mutex battleMutex;

void battle(Character& hero, Character& monster) {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(battleMutex);
            if (!hero.isAlive() || !monster.isAlive()) break;

            monster.takeDamage(hero.getAttack());
            std::cout << hero.getName() << " hits " << monster.getName() << "!\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            if (!monster.isAlive()) break;

            hero.takeDamage(monster.getAttack());
            std::cout << monster.getName() << " hits " << hero.getName() << "!\n";
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::lock_guard<std::mutex> lock(battleMutex);
    if (!hero.isAlive()) {
        std::cout << hero.getName() << " has been defeated!\n";
    }
    else if (!monster.isAlive()) {
        std::cout << monster.getName() << " has been defeated!\n";
    }
}

int main() {
    Character hero("Hero", 100, 20, 5);
    Character monster("Orc", 80, 15, 3);

    hero.displayInfo();
    monster.displayInfo();

    std::thread fightThread(battle, std::ref(hero), std::ref(monster));
    fightThread.join();

    hero.displayInfo();
    monster.displayInfo();

    return 0;
}
