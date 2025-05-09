#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Inventory {
private:
    std::unique_ptr<std::string[]> items;
    int capacity;
    int count;

public:
    Inventory(int cap) : capacity(cap), count(0) {
        items = std::make_unique<std::string[]>(capacity);
    }

    void addItem(const std::string& item) {
        if (count < capacity) {
            items[count++] = item;
        }
        else {
            std::cout << "Inventory is full!\n";
        }
    }

    void displayInventory() const {
        std::cout << "Inventory:\n";
        for (int i = 0; i < count; ++i) {
            std::cout << "- " << items[i] << "\n";
        }
    }
};

int main() {
    Inventory inv(5);
    inv.addItem("Sword");
    inv.addItem("Health Potion");
    inv.addItem("Shield");
    inv.displayInventory();

    return 0;
}
