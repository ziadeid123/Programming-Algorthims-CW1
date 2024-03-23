#include <iostream>// For standard input/output operations (cin, cout, cerr)
#include <fstream> // For file input/output operations
#include <sstream> // For string stream operations
#include <vector>  // For dynamic arrays (std::vector)
#include <algorithm>// For common algorithms (e.g., sort, find)
#include <cstring> // For C-style string manipulation functions (e.g., strcpy, strlen)
#include <cstdlib>// For general utility functions (e.g., atoi, atof)
#include <ctime> // For time-related functions (e.g., time, clock)

//  hash table implementation
class HashTable {
private:
    static const int TABLE_SIZE = 100; // Fixed size of the hash table
    // Structure to hold key-value pairs and a pointer to the next node
  // for handling collisions
    struct Node { 
        std::string key;
        std::string value;
        Node* next; // Pointer to the next node in the linked list
    };

    Node* table[TABLE_SIZE]; // Array of pointers to Nodes, representing the hash table

    int hashFunction(const std::string& key) {  // Hash function to map keys to indices in the table
        int hash = 0;
        for (char c : key) {
            hash += c; // Simple hash calculation by summing character values
        }
        return hash % TABLE_SIZE; // Modulo operation to distribute indices
    }

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {   // Constructor to initialize the hash table
            table[i] = nullptr; // Set all table entries to null initially
        }
    }

    void insert(const std::string& key, const std::string& value) {
        int index = hashFunction(key);   // Calculate the hash index for the given key
        Node* newNode = new Node;  // Create a new Node to store the key-value pair
        newNode->key = key;
        newNode->value = value;
        newNode->next = nullptr; // Append it to the end of the linked list
        if (table[index] == nullptr) {// Check for collisions at the calculated index
            table[index] = newNode; // No collision, insert directly
        } else {
            Node* temp = table[index]; // Collision, handle by chaining
            while (temp->next != nullptr) {
                temp = temp->next;// Traverse to the end of the list
            }
            temp->next = newNode; // Append the new node
        }
    }

    std::string retrieve(const std::string& key) {
        int index = hashFunction(key); // Get the hash index for the key
        Node* temp = table[index];  // Start searching from the head of the linked list at the index
        while (temp != nullptr) {
            if (temp->key == key) {  // Key found, return its value
                return temp->value;
            }
            temp = temp->next;// Move to the next node in the list
        }
        return ""; // Key not found, return an empty string
    }
};
// Caesar cipher encryption function
std::string encrypt(const std::string &data) {
    std::string encrypted;
    for (char c : data) {
        encrypted += c + 1; // Shift each character by 1 (Caesar cipher)
    }
    return encrypted;
}
//  Caesar cipher decryption function
std::string decrypt(const std::string &data) {
    std::string decrypted;
    for (char c : data) {
        decrypted += c - 1; // Shift each character back by 1 (reverse Caesar cipher)
    }
    return decrypted;
}

class PasswordManager { // Class representing a password manager
private:
    std::string masterPassword; // Master password used to access stored passwords
    HashTable passwords;  // Hash table to store website/username and password pairs

    std::string encryptPassword(const std::string &password) { // Encrypts a password using a simple Caesar cipher
        return encrypt(password);
    }
// Decrypts an encrypted password using a simple Caesar cipher
    std::string decryptPassword(const std::string &encryptedPassword) {
        return decrypt(encryptedPassword);
    }

public:
    PasswordManager() : masterPassword("ziad") {}
// Default constructor setting the master password
    bool authenticate(const std::string &password) {
        return password == masterPassword;
    }
// Authenticates a user by checking if the provided password matches the master password
// Generates a random password of specified length using a character set
    std::string generatePassword(int length) {
        std::string charset =  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::string generatedPassword;
        srand(time(nullptr)); // Seed the random number generator
        for (int i = 0; i < length; ++i) {
            generatedPassword += charset[rand() % charset.length()];// Select random character from the set
        }
        return generatedPassword;
    }

    void savePassword(const std::string &website, const std::string &username, const std::string &password, const std::string &filename) { // Encrypt website, username, and password for secure storage
        std::string encryptedWebsite = encrypt(website);
        std::string encryptedUsername = encrypt(username);
        std::string encryptedPassword = encryptPassword(password); // Combine encrypted website and username as a key
        std::string key = encryptedWebsite + "," + encryptedUsername;
        passwords.insert(key, encryptedPassword); // Insert encrypted password into a secure storage mechanism 
        // Open file for appending (creates if non-existent)
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            file << encryptedWebsite << "," << encryptedUsername << "," << encryptedPassword << "\n";
            file.close();
            std::cout << "Password saved successfully.\n";
        } else {
            std::cerr << "Error: Unable to open file for writing.\n";
        }
    }

    void retrievePassword(const std::string &website, const std::string &username) {// Encrypt website and username for secure comparison
        std::string encryptedWebsite = encrypt(website);
        std::string encryptedUsername = encrypt(username);
        std::string key = encryptedWebsite + "," + encryptedUsername;

        std::string encryptedPassword = passwords.retrieve(key);
        if (!encryptedPassword.empty()) { // Check if password was found
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
                std::string encryptedWebsite, encryptedUsername, encryptedPassword;// Extract comma-separated values (ensure proper parsing logic for security)
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
    std::cout << "Enter master password: "; // Prompt user for master password for authentication
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
