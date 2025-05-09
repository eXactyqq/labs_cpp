#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

class Entity {
protected:
    std::string name;
    int health;
    int level;

public:
    Entity(std::string n, int h, int l) : name(n), health(h), level(l) {}
    virtual ~Entity() = default;

    virtual void display() const {
        std::cout << "Name: " << name << ", Health: " << health << ", Level: " << level << std::endl;
    }

    virtual std::string getType() const = 0;

    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getLevel() const { return level; }

    void setName(const std::string& n) { name = n; }
    void setHealth(int h) { health = h; }
    void setLevel(int l) { level = l; }
};

class Player : public Entity {
public:
    Player(std::string n, int h, int l) : Entity(n, h, l) {}
    std::string getType() const override { return "Player"; }
};
template<typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    void addEntity(T entity) {
        entities.push_back(entity);
    }

    const std::vector<T>& getEntities() const {
        return entities;
    }

    void displayAll() const {
        for (const auto& e : entities) {
            e->display();
        }
    }

    void clear() {
        for (auto e : entities)
            delete e;
        entities.clear();
    }

    ~GameManager() {
        clear();
    }
};
void saveToFile(const GameManager<Entity*>& manager, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing.");
    }

    for (const auto& entity : manager.getEntities()) {
        file << entity->getType() << ' '
            << entity->getName() << ' '
            << entity->getHealth() << ' '
            << entity->getLevel() << '\n';
    }

    file.close();
}

void loadFromFile(GameManager<Entity*>& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading.");
    }

    std::string type, name;
    int health, level;

    while (file >> type >> name >> health >> level) {
        if (type == "Player") {
            manager.addEntity(new Player(name, health, level));
        }
    }

    file.close();
}
int main() {
    try {
        GameManager<Entity*> manager;
        manager.addEntity(new Player("Hero", 100, 1));
        manager.addEntity(new Player("Knight", 80, 2));
        manager.addEntity(new Player("Mage", 60, 3));

        saveToFile(manager, "game_save.txt");

        GameManager<Entity*> loadedManager;
        loadFromFile(loadedManager, "game_save.txt");

        std::cout << "Loaded characters:\n";
        loadedManager.displayAll();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
