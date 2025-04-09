#include <string>
#include <cstdlib> 
#include <ctime>   
#include <iostream>

class Entity {
protected:
    std::string name;
    int health;
    int attackPower;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attackPower(a), defense(d) {
    }

    std::string getName() const {
        return name;
    }

    int getHealth() const { return health; }
    int getDefense() const { return defense; }
    void reduceHealth(int amount) { health -= amount; }

    virtual void attack(Entity& target) {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }

    virtual void heal(int amount) {
        if (amount > 0) {
            health += amount;
            std::cout << name << " restores " << amount << " HP!\n";
        }
    }

    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 20) {
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "  Character: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
    void heal(int amount) override {
        if (health == 100)
        {
            std::cout << name << "already full hp!" << std::endl;
            return;
        }
        health += amount;
        if (health > 100) {
            health = 100;
        }
        std::cout << "  " << name << " healed for " << amount << ". Current hp: " << health << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {
    }

    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 30) {
                damage += 5; 
                std::cout << "Poisonous attack! ";
            }
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "  Monster: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};

class Boss : public Monster {
public:
    Boss(const std::string& n, int h, int a, int d)
        : Monster(n, h, a, d) {
    }
    void attack(Entity& target) override {
        int damage = attackPower - target.getDefense();
        if (damage > 0) {
            if (rand() % 100 < 60) {
                damage += 10;
                std::cout << "Fear! ";
                target.reduceHealth(damage);
                std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
                return;
            }
            target.reduceHealth(damage);
            std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.getName() << ", but it has no effect!\n";
            return;
        }
    }
    void displayInfo() const override {
        std::cout << "  Boss: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }
};


int main() {
    srand(static_cast<unsigned>(time(0))); 

    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss demon("Nevermore", 150, 25, 20);

    Entity* entities[] = { &hero, &goblin, &demon };

    for (auto* entity : entities) {
        entity->displayInfo(); 
    }

    hero.attack(goblin);
    goblin.attack(hero);
    demon.attack(hero);

    std::cout << std::endl;
    hero.displayInfo();
    goblin.displayInfo();
    demon.displayInfo();

    hero.heal(30);


    return 0;
}