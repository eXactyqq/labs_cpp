#include <iostream>
#include <vector>
#include <stdexcept>

template <typename T>
class Queue {
private:
    std::vector<T> data;

public:
    void push(const T& item) {
        data.push_back(item);
    }

    void pop() {
        if (!data.empty()) {
            data.erase(data.begin());
        }
        else {
            std::cout << "Queue is empty!\n";
        }
    }

    void display() const {
        std::cout << "Queue: ";
        for (const auto& item : data) {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }

    bool isEmpty() const {
        return data.empty();
    }
};

int main() {
    Queue<std::string> stringQueue;
    stringQueue.push("apple");
    stringQueue.push("banana");
    stringQueue.push("cherry");
    stringQueue.display();
    stringQueue.pop();
    stringQueue.display();

    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);
    intQueue.display();
    intQueue.pop();
    intQueue.display();

    return 0;
}
