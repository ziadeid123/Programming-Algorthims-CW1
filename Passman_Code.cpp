#include <iostream> // For input/output operations like cout and cin
#include <cstring>  // For string manipulation functions
#include <cstdlib>  // For random number generation using rand and srand
#include <ctime>    // For time-related functions like time and clock
#include <vector>   // For using dynamic arrays called vectors
#include <algorithm> // for std::find

// // Class for a hash table implementation
class HashTable {
private:
    static const int TABLE_SIZE = 100; // Fixed size of the hash table
    struct Node { // Node structure to store key-value pairs
        std::string key;
        std::string value;
        Node* next; // Pointer to handle collisions 
    };

    Node* table[TABLE_SIZE]; // Array of nodes to hold the table
// Hash function to map keys to indices
    int hashFunction(const std::string& key) { 
        int hash = 0;
        for (char c : key) { // Calculate a simple hash using character sums
            hash += c;
        }
        return hash % TABLE_SIZE; // Modulo for table size
    }

public:
// Constructor to initialize the table
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = nullptr; // Initially set all table entries to empty
        }
    }
// Insert a key-value pair into the hash table
    void insert(const std::string& key, const std::string& value) {
        int index = hashFunction(key);// Get the index for the key
        Node* newNode = new Node; // Create a new node for the key-value pair
        newNode->key = key;
        newNode->value = value;
        newNode->next = nullptr; // Initially set its next pointer to null
// Handle collisions with separate chaining
        if (table[index] == nullptr) {
            table[index] = newNode; // If the slot is empty, insert directly
        } else {
            Node* temp = table[index];
            while (temp->next != nullptr) { // Traverse the chain to find the end
                temp = temp->next;
            }
            temp->next = newNode; // Append the new node to the end of the chain
        }
    }
//Retrieve the value associated with a given key
    std::string retrieve(const std::string& key) { 
        int index = hashFunction(key); // Get the index for the key
        Node* temp = table[index];
        while (temp != nullptr) { // Traverse the chain at the index
            if (temp->key == key) {
                return temp->value; // Return the value if the key is found
            }
            temp = temp->next;
        }
        return ""; // Return empty string if key not found
    }
};

// Caesar cipher encryption and decryption functions
std::string encrypt(const std::string &data) {
    std::string encrypted; //Initialize empty string for encrypted data
    for (char c : data) {
        encrypted += c + 1; // Shift each character by 1 position (Caesar cipher with key = 1)
    }
    return encrypted; // Return the encrypted string
}
//Caesar cipher decryption function
std::string decrypt(const std::string &data) {
    std::string decrypted;// Initialize empty string for decrypted data
    for (char c : data) {
        decrypted += c - 1; // Shift each character back by 1 position to reverse the encryption
    }
    return decrypted; // Return the decrypted string
}

class PasswordManager {
private:
    std::string masterPassword; //Master password used to encrypt and decrypt stored passwords
    HashTable passwords;// Hash table to store website/username pairs and their encrypted passwords
// Utility function to encrypt a password using the Caesar cipher (for demonstration)
    std::string encryptPassword(const std::string &password) {
        return encrypt(password); // Call the provided encryption function
    }
// Utility function to decrypt a password using the Caesar cipher (for demonstration)
    std::string decryptPassword(const std::string &encryptedPassword) {
        return decrypt(encryptedPassword); // Call the provided decryption function
    }

public:  // Constructor to initialize the PasswordManager with the master password
    PasswordManager(const std::string &masterPass) : masterPassword(masterPass) {}

    bool authenticate(const std::string &password) {// Function to authenticate the user
        return password == "ziad";
    }
// Generates a random password of the specified length.
    std::string generatePassword(int length) {
        // Character set containing lowercase, uppercase letters, and digits
        std::string charset =  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::string generatedPassword;
        srand(time(nullptr)); // Seed the random number generator for creating unpredictable passwords.
        for (int i = 0; i < length; ++i) { // Loop to construct the password by randomly selecting characters.
            generatedPassword += charset[rand() % charset.length()];
        }
        return generatedPassword;
    }
// Saves a password for a website and username, encrypting sensitive information.
    void savePassword(const std::string &website, const std::string &username, const std::string &password) {
        std::string encryptedWebsite = encrypt(website);// Encrypt the website and username for enhanced security.
        std::string encryptedUsername = encrypt(username);
        std::string encryptedPassword = encryptPassword(password);
// Create a unique key to identify the password for the website and username.
        std::string key = encryptedWebsite + "," + encryptedUsername;
        passwords.insert(key, encryptedPassword);  // Store the encrypted password in the passwords data store.

        std::cout << "Password saved successfully.\n"; // Inform the user about successful password saving.
    }
// Retrieves a password for a website and username, decrypting it if found.
    void retrievePassword(const std::string &website, const std::string &username) {
        std::string encryptedWebsite = encrypt(website);// Encrypt the website and username for consistency with storage format.
        std::string encryptedUsername = encrypt(username);
        std::string key = encryptedWebsite + "," + encryptedUsername;// Create the key to locate the password in the data store.

        std::string encryptedPassword = passwords.retrieve(key);
        if (!encryptedPassword.empty()) {// Attempt to retrieve the encrypted password from the data store.
            std::cout << "Password: " << decryptPassword(encryptedPassword) << std::endl;//If found, decrypt the password and display it to the user.
        } else {
            std::cerr << "Password not found.\n";// If not found, inform the user.
        }
    }
};

int main() { // Main entry point for the password manager application.
    PasswordManager manager("masterpassword");

    std::string password;
    std::cout << "Enter master password: "; // Prompt the user for the master password and authenticate.
    std::cin >> password;

    if (manager.authenticate(password)) {
        std::cout << "Authentication successful.\n";
        std::string choice;// Main loop for password management operations.
        do {// Display a menu of options for the user.
            std::cout << "\n1. Generate password\n2. Retrieve password\n3. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            if (choice == "1") {// Generate a password for a website and username.
                std::string website, username;
                std::cout << "Enter website: ";
                std::cin >> website;
                std::cout << "Enter username: ";
                std::cin >> username;

                std::string generatedPassword = manager.generatePassword(12);
                std::cout << "Generated password: " << generatedPassword << std::endl;

                manager.savePassword(website, username, generatedPassword);
            } else if (choice == "2") {// Retrieve a saved password for a website and username.
                std::string website, username;
                std::cout << "Enter website: ";
                std::cin >> website;
                std::cout << "Enter username: ";
                std::cin >> username;
                manager.retrievePassword(website, username);
            }

        } while (choice != "3");// Continue the loop until the user chooses to exit.
    } else {
        std::cerr << "Authentication failed.\n";
    }

    return 0;
}