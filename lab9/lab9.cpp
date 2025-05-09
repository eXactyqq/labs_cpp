#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;
    int level;
    int experience;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d), level(1), experience(0) {}

    void attackEnemy(class Monster& enemy);

    void heal(int amount) {
        health += amount;
        if (health > 100) health = 100;
        std::cout << name << " heals for " << amount << " HP!\n";
    }

    void gainExperience(int exp) {
        experience += exp;
        if (experience >= 100) {
            level++;
            experience -= 100;
            std::cout << name << " leveled up to level " << level << "!\n";
        }
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense
            << ", Level: " << level << ", Experience: " << experience << "\n";
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    void reduceHealth(int damage) {
        health -= damage;
        if (health < 0) throw std::runtime_error(name + " has been defeated.");
    }

    void save(std::ofstream& out) {
        out << name << " " << health << " " << attack << " " << defense << " " << level << " " << experience << "\n";
    }

    void load(std::ifstream& in) {
        in >> name >> health >> attack >> defense >> level >> experience;
    }
};

class Monster {
protected:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    virtual ~Monster() = default;

    virtual void attackPlayer(Character& player) {
        int damage = attack - player.getDefense();
        if (damage > 0) {
            player.reduceHealth(damage);
            std::cout << name << " attacks " << player.getName() << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks but it has no effect!\n";
        }
    }

    virtual void displayInfo() const {
        std::cout << "Monster: " << name << ", HP: " << health << ", Attack: " << attack << ", Defense: " << defense << "\n";
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getDefense() const { return defense; }

    void reduceHealth(int damage) {
        health -= damage;
        if (health < 0) throw std::runtime_error(name + " has been defeated.");
    }
};

class Goblin : public Monster {
public:
    Goblin() : Monster("Goblin", 30, 10, 5) {}
};

class Dragon : public Monster {
public:
    Dragon() : Monster("Dragon", 100, 25, 15) {}
};

class Skeleton : public Monster {
public:
    Skeleton() : Monster("Skeleton", 40, 15, 10) {}
};

void Character::attackEnemy(Monster& enemy) {
    int damage = attack - enemy.getDefense();
    if (damage > 0) {
        enemy.reduceHealth(damage);
        std::cout << name << " attacks " << enemy.getName() << " for " << damage << " damage!\n";
    }
    else {
        std::cout << name << " attacks but it has no effect!\n";
    }
}

class Inventory {
private:
    std::vector<std::string> items;

public:
    void addItem(const std::string& item) {
        items.push_back(item);
    }

    void removeItem(const std::string& item) {
        items.erase(std::remove(items.begin(), items.end(), item), items.end());
    }

    void showInventory() const {
        std::cout << "Inventory:\n";
        for (const auto& item : items) {
            std::cout << "- " << item << "\n";
        }
    }

    void save(std::ofstream& out) {
        out << items.size() << "\n";
        for (const auto& item : items)
            out << item << "\n";
    }

    void load(std::ifstream& in) {
        size_t size;
        in >> size;
        in.ignore();
        items.clear();
        for (size_t i = 0; i < size; ++i) {
            std::string item;
            std::getline(in, item);
            items.push_back(item);
        }
    }
};

template<typename T>
class Logger {
private:
    std::string filename;

public:
    Logger(const std::string& fname) : filename(fname) {}

    void log(const T& message) {
        std::ofstream out(filename, std::ios::app);
        if (out) out << message << "\n";
    }
};

class Game {
private:
    Character player;
    Inventory inventory;
    Logger<std::string> logger;

public:
    Game() : player("Hero", 100, 20, 10), logger("game_log.txt") {}

    void start() {
        logger.log("Game started.");
        player.displayInfo();
        inventory.addItem("Potion");
        inventory.addItem("Sword");
        inventory.showInventory();
        fight(Goblin());
        fight(Skeleton());
        save();
        load();
    }

    void fight(Monster enemy) {
        logger.log("Battle started with " + enemy.getName());
        try {
            while (player.getHealth() > 0 && enemy.getHealth() > 0) {
                player.attackEnemy(enemy);
                if (enemy.getHealth() > 0)
                    enemy.attackPlayer(player);
            }
            player.gainExperience(50);
            logger.log("Battle ended with " + enemy.getName());
        }
        catch (std::exception& e) {
            logger.log(e.what());
            std::cout << e.what() << "\n";
        }
    }

    void save() {
        std::ofstream out("save.txt");
        if (!out) throw std::runtime_error("Failed to save game.");
        player.save(out);
        inventory.save(out);
        logger.log("Game saved.");
    }

    void load() {
        std::ifstream in("save.txt");
        if (!in) throw std::runtime_error("Failed to load game.");
        player.load(in);
        inventory.load(in);
        logger.log("Game loaded.");
        player.displayInfo();
        inventory.showInventory();
    }
};

int main() {
    try {
        Game game;
        game.start();
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
