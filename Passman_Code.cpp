#include <iostream>
#include <fstream> // For file input/output operations
#include <sstream> // For string stream operations
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <ctime>

// Simple hash table implementation
class HashTable {
private:
    static const int TABLE_SIZE = 100;
    struct Node {
        std::string key;
        std::string value;
        Node* next;
    };

    Node* table[TABLE_SIZE];

    int hashFunction(const std::string& key) {
        int hash = 0;
        for (char c : key) {
            hash += c;
        }
        return hash % TABLE_SIZE;
    }

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = nullptr;
        }
    }

    void insert(const std::string& key, const std::string& value) {
        int index = hashFunction(key);
        Node* newNode = new Node;
        newNode->key = key;
        newNode->value = value;
        newNode->next = nullptr;
        if (table[index] == nullptr) {
            table[index] = newNode;
        } else {
            Node* temp = table[index];
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    std::string retrieve(const std::string& key) {
        int index = hashFunction(key);
        Node* temp = table[index];
        while (temp != nullptr) {
            if (temp->key == key) {
                return temp->value;
            }
            temp = temp->next;
        }
        return "";
    }
};

std::string encrypt(const std::string &data) {
    std::string encrypted;
    for (char c : data) {
        encrypted += c + 1;
    }
    return encrypted;
}

std::string decrypt(const std::string &data) {
    std::string decrypted;
    for (char c : data) {
        decrypted += c - 1;
    }
    return decrypted;
}

class PasswordManager {
private:
    std::string masterPassword;
    HashTable passwords;

    std::string encryptPassword(const std::string &password) {
        return encrypt(password);
    }

    std::string decryptPassword(const std::string &encryptedPassword) {
        return decrypt(encryptedPassword);
    }

public:
    PasswordManager() : masterPassword("ziad") {}

    bool authenticate(const std::string &password) {
        return password == masterPassword;
    }

    std::string generatePassword(int length) {
        std::string charset =  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::string generatedPassword;
        srand(time(nullptr));
        for (int i = 0; i < length; ++i) {
            generatedPassword += charset[rand() % charset.length()];
        }
        return generatedPassword;
    }

    void savePassword(const std::string &website, const std::string &username, const std::string &password, const std::string &filename) {
        std::string encryptedWebsite = encrypt(website);
        std::string encryptedUsername = encrypt(username);
        std::string encryptedPassword = encryptPassword(password);
        std::string key = encryptedWebsite + "," + encryptedUsername;
        passwords.insert(key, encryptedPassword);
        
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            file << encryptedWebsite << "," << encryptedUsername << "," << encryptedPassword << "\n";
            file.close();
            std::cout << "Password saved successfully.\n";
        } else {
            std::cerr << "Error: Unable to open file for writing.\n";
        }
    }

    void retrievePassword(const std::string &website, const std::string &username) {
        std::string encryptedWebsite = encrypt(website);
        std::string encryptedUsername = encrypt(username);
        std::string key = encryptedWebsite + "," + encryptedUsername;

        std::string encryptedPassword = passwords.retrieve(key);
        if (!encryptedPassword.empty()) {
            std::cout << "Password: " << decryptPassword(encryptedPassword) << std::endl;
        } else {
            std::cerr << "Password not found.\n";
        }
    }

    void loadPasswordsFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string encryptedWebsite, encryptedUsername, encryptedPassword;
                if (std::getline(iss, encryptedWebsite, ',') &&
                    std::getline(iss, encryptedUsername, ',') &&
                    std::getline(iss, encryptedPassword)) {
                    passwords.insert(encryptedWebsite + "," + encryptedUsername, encryptedPassword);
                }
            }
            file.close();
        } else {
            std::cerr << "Error: Unable to open file for reading.\n";
        }
    }
};

int main() {
    PasswordManager manager;
    std::string password;
    std::cout << "Enter master password: ";
    std::cin >> password;

    if (manager.authenticate(password)) {
        std::cout << "Authentication successful.\n";
        manager.loadPasswordsFromFile("passwords.txt");
        std::string choice;
        do {
            std::cout << "\n1. Generate password\n2. Retrieve password\n3. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            if (choice == "1") {
                std::string website, username;
                std::cout << "Enter website: ";
                std::cin >> website;
                std::cout << "Enter username: ";
                std::cin >> username;

                std::string generatedPassword = manager.generatePassword(12);
                std::cout << "Generated password: " << generatedPassword << std::endl;

                manager.savePassword(website, username, generatedPassword, "passwords.txt");
            } else if (choice == "2") {
                std::string website, username;
                std::cout << "Enter website: ";
                std::cin >> website;
                std::cout << "Enter username: ";
                std::cin >> username;
                manager.retrievePassword(website, username);
            }

        } while (choice != "3");
    } else {
        std::cerr << "Authentication failed.\n";
    }

    return 0;
}
