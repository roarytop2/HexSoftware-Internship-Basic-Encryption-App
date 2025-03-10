#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <limits>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;

// Function prototypes
void handleInput();
void handleTxt();
string encrypt(const string& plain_text, int key);
string decrypt(const string& cipher_text, int key);
int getValidKey();

int main() {
    try {
        cout << "Choose the input format:\n1. User\n2. Text file\nChoose (1/2): ";
        char choice;
        cin >> choice;
        // Clear leftover newline characters
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == '1') {
            handleInput();
        } else if (choice == '2') {
            handleTxt();
        } else {
            throw std::invalid_argument("Invalid choice. Please restart and choose either 1 or 2.");
        }
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}

// Helper function to obtain a valid key from the user
int getValidKey() {
    int key;
    while (true) {
        cout << "Please enter the shifting key in the range (0 - 25): ";
        cin >> key;
        if (cin.fail()) {
            // Clear error flags and ignore rest of input line
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Invalid input. Please enter an integer.\n";
            continue;
        }
        if (key < 0 || key > 25) {
            cout << "Key must be between 0 and 25. Try again.\n";
            // Clear any leftover characters
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        // Clear any extra input and return valid key
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return key;
    }
}

void handleInput() {
    string input_str, output_str, action_str;
    cout << "Would you like to encrypt or decrypt text today?\n";
    std::getline(cin, action_str);
    
    // Convert action_str to lowercase for case-insensitive comparison
    std::transform(action_str.begin(), action_str.end(), action_str.begin(), ::tolower);
    
    if (action_str == "encrypt") {
        cout << "Enter the plain text:\n";
        std::getline(cin, input_str);
        int key = getValidKey();
        output_str = encrypt(input_str, key);
        cout << "The cipher text is:\n" << output_str << endl;
    } else if (action_str == "decrypt") {
        cout << "Enter the cipher text:\n";
        std::getline(cin, input_str);
        int key = getValidKey();
        output_str = decrypt(input_str, key);
        cout << "The plain text is:\n" << output_str << endl;
    } else {
        cout << "Invalid action requested. Please choose either \"encrypt\" or \"decrypt\".\n";
    }
}

void handleTxt() {// handles text file input

    string filename;
    string content;
    string action_str, output_str;
    
    cout << "Enter the text file name: ";
    std::getline(cin, filename);
    
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    // Read entire file content
    content.assign((std::istreambuf_iterator<char>(inputFile)),
                   std::istreambuf_iterator<char>());
    inputFile.close();
    
    cout << "Would you like to encrypt or decrypt the file? ";
    std::getline(cin, action_str);
    std::transform(action_str.begin(), action_str.end(), action_str.begin(), ::tolower);
    
    if (action_str == "encrypt") {
        int key = getValidKey();
        output_str = encrypt(content, key);
    } else if (action_str == "decrypt") {
        int key = getValidKey();
        output_str = decrypt(content, key);
    } else {
        throw std::invalid_argument("Invalid action. Please choose either \"encrypt\" or \"decrypt\".");
    }
    
    cout << "Save as:\n1. Same file\n2. New file\nChoose (1/2): ";
    char choice;
    cin >> choice;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (choice == '1') {
        std::ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }
        outputFile << output_str;
        if (!outputFile) {
            throw std::runtime_error("Error writing to file: " + filename);
        }
        outputFile.close();
        cout << "File overwritten successfully!\n";
    } else if (choice == '2') {
        string newFilename;
        cout << "Enter new filename: ";
        std::getline(cin, newFilename);
        std::ofstream outputFile(newFilename);
        if (!outputFile.is_open()) {
            throw std::runtime_error("Cannot open new file for writing: " + newFilename);
        }
        outputFile << output_str;
        if (!outputFile) {
            throw std::runtime_error("Error writing to file: " + newFilename);
        }
        outputFile.close();
        cout << "New file created successfully!\n";
    } else {
        throw std::invalid_argument("Invalid choice for saving file.");
    }
}

string encrypt(const string& plain_text, int key) {
    string cipher_text;
    for (char letter : plain_text) {
        if (std::isalpha(letter)) {
            if (std::isupper(letter)) {
                cipher_text += 'A' + (letter - 'A' + key) % 26;
            } else {
                cipher_text += 'a' + (letter - 'a' + key) % 26;
            }
        } else {
            cipher_text += letter;
        }
    }
    return cipher_text;
}

string decrypt(const string& cipher_text, int key) {
    string plain_text;
    for (char letter : cipher_text) {
        if (std::isalpha(letter)) {
            if (std::isupper(letter)) {
                plain_text += 'A' + (letter - 'A' - key + 26) % 26;
            } else {
                plain_text += 'a' + (letter - 'a' - key + 26) % 26;
            }
        } else {
            plain_text += letter;
        }
    }
    return plain_text;
}

