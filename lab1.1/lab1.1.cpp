#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;
    const int maxHealth = 100;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    int getHealth() const {
        return health;
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.takeDamage(damage);
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }

    void heal(int amount) {
        health += amount;
        std::cout << name << " recieved " << amount << " health!" << std::endl;
        if (health > maxHealth) {
            health = maxHealth;
        }
    }

    void takeDamage(int amount) {
        health -= amount;
        std::cout << name << " wounded for " << amount << " health!" << std::endl;
        if (health < 0) {
            health = 0;
        }
    }
};

int main() {
    Character hero("Hero", 80, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    hero.displayInfo();
    monster.displayInfo();

    hero.attackEnemy(monster);
    monster.displayInfo();

    monster.attackEnemy(hero);
    hero.displayInfo();

    hero.heal(30);
    hero.displayInfo();

    monster.takeDamage(60);
    monster.displayInfo();

    return 0;
}
