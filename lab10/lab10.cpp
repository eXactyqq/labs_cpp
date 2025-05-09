#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <algorithm>

class User {
private:
    std::string name;
    int id;
    int accessLevel;

public:
    User(const std::string& name, int id, int accessLevel) {
        if (name.empty()) throw std::invalid_argument("Name cannot be empty.");
        if (id < 0) throw std::invalid_argument("ID cannot be negative.");
        if (accessLevel < 0) throw std::invalid_argument("Access level cannot be negative.");
        this->name = name;
        this->id = id;
        this->accessLevel = accessLevel;
    }

    virtual ~User() = default;

    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", ID: " << id << ", Access Level: " << accessLevel << "\n";
    }

    virtual void save(std::ofstream& out) const {
        out << name << " " << id << " " << accessLevel << "\n";
    }
};

class Student : public User {
private:
    std::string group;

public:
    Student(const std::string& name, int id, int accessLevel, const std::string& group)
        : User(name, id, accessLevel), group(group) {}

    void displayInfo() const override {
        std::cout << "Student - ";
        User::displayInfo();
        std::cout << "Group: " << group << "\n";
    }

    void save(std::ofstream& out) const override {
        out << "Student ";
        User::save(out);
        out << group << "\n";
    }
};

class Teacher : public User {
private:
    std::string department;

public:
    Teacher(const std::string& name, int id, int accessLevel, const std::string& department)
        : User(name, id, accessLevel), department(department) {}

    void displayInfo() const override {
        std::cout << "Teacher - ";
        User::displayInfo();
        std::cout << "Department: " << department << "\n";
    }

    void save(std::ofstream& out) const override {
        out << "Teacher ";
        User::save(out);
        out << department << "\n";
    }
};

class Administrator : public User {
private:
    std::string role;

public:
    Administrator(const std::string& name, int id, int accessLevel, const std::string& role)
        : User(name, id, accessLevel), role(role) {}

    void displayInfo() const override {
        std::cout << "Administrator - ";
        User::displayInfo();
        std::cout << "Role: " << role << "\n";
    }

    void save(std::ofstream& out) const override {
        out << "Administrator ";
        User::save(out);
        out << role << "\n";
    }
};

class Resource {
private:
    std::string name;
    int requiredAccessLevel;

public:
    Resource(const std::string& name, int level) : name(name), requiredAccessLevel(level) {}

    std::string getName() const { return name; }

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    void display() const {
        std::cout << "Resource: " << name << ", Required Access Level: " << requiredAccessLevel << "\n";
    }

    void save(std::ofstream& out) const {
        out << name << " " << requiredAccessLevel << "\n";
    }
};

template<typename TUser, typename TResource>
class AccessControlSystem {
private:
    std::vector<std::shared_ptr<TUser>> users;
    std::vector<TResource> resources;

public:
    void addUser(std::shared_ptr<TUser> user) {
        users.push_back(user);
    }

    void addResource(const TResource& resource) {
        resources.push_back(resource);
    }

    void showAllUsers() const {
        for (const auto& user : users)
            user->displayInfo();
    }

    void showAllResources() const {
        for (const auto& res : resources)
            res.display();
    }

    void checkAllAccess() const {
        for (const auto& user : users) {
            for (const auto& res : resources) {
                std::cout << user->getName() << (res.checkAccess(*user) ? " has access to " : " cannot access ") << res.getName() << "\n";
            }
        }
    }

    std::shared_ptr<TUser> findUserById(int id) const {
        auto it = std::find_if(users.begin(), users.end(), [id](const auto& u) { return u->getId() == id; });
        return it != users.end() ? *it : nullptr;
    }

    void sortUsersByAccessLevel() {
        std::sort(users.begin(), users.end(), [](const auto& a, const auto& b) {
            return a->getAccessLevel() > b->getAccessLevel();
            });
    }

    void saveToFile(const std::string& userFile, const std::string& resourceFile) {
        std::ofstream userOut(userFile), resOut(resourceFile);
        if (!userOut || !resOut) throw std::runtime_error("Cannot open file for saving.");
        for (const auto& user : users) user->save(userOut);
        for (const auto& res : resources) res.save(resOut);
    }
};

int main() {
    try {
        AccessControlSystem<User, Resource> system;

        system.addUser(std::make_shared<Student>("Alex", 1, 1, "gd23"));
        system.addUser(std::make_shared<Teacher>("Leo", 2, 3, "math"));
        system.addUser(std::make_shared<Administrator>("John", 3, 5, "adm"));

        system.addResource(Resource("101", 1));
        system.addResource(Resource("202", 2));
        system.addResource(Resource("303", 5));

        std::cout << "All users:\n";
        system.showAllUsers();

        std::cout << "\nAll resources:\n";
        system.showAllResources();

        std::cout << "\nAccess check:\n";
        system.checkAllAccess();

        std::cout << "\nSorted users by access level:\n";
        system.sortUsersByAccessLevel();
        system.showAllUsers();

        system.saveToFile("users.txt", "resources.txt");
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }

    return 0;
}
