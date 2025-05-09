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
        if (data.empty()) {
            throw std::out_of_range("Queue is empty. Cannot pop.");
        }
        data.erase(data.begin());
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
    Queue<int> intQueue;

    try {
        intQueue.push(10);
        intQueue.push(20);
        intQueue.display();

        intQueue.pop();
        intQueue.pop();
        intQueue.pop(); 
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    Queue<std::string> strQueue;

    try {
        strQueue.pop(); 
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
