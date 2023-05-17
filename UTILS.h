#ifndef UTILS_H
#define UTILS_H

#include <vector>

// Checks if a string contains any whitespace characters in it using a regular expression.
bool noWhitespace(const std::string &number) {
    std::regex pattern(R"(^\s*-?\w+\s*$)");
    return regex_match(number, pattern);
}

// Reads a line of input and returns it as a string.
std::string getLine() {
    std::string input;
    getline(std::cin, input);
    if (input == "\n" or input.empty()) {
        getline(std::cin, input);
    }
    return input;
}

// Reads an integer within the specified range (default is all integers).
// Prompts the user to enter a valid integer with no whitespace, and retry if the entered value is invalid.
// Throws an out_of_range exception if the entered value is out of the specified range.
int getInt(int min = std::numeric_limits<int>::min(), int max = std::numeric_limits<int>::max()) {
    int input;
    std::string inputStr;
    while (true) {
        inputStr = getLine();
        if (!noWhitespace(inputStr)) {
            std::cout << "Please enter a number with no whitespace: ";
            continue;
        }
        try {
            input = stoi(inputStr);
            if (input < min or input > max)
                throw (std::out_of_range("Please enter an integer between "
                                         + std::to_string(min) + " and " + std::to_string(max) + ": "));
            return input;
        }
        catch (std::invalid_argument &e) {
            std::cout << "Error: Please enter an integer: ";
        }
        catch (std::out_of_range &e) {
            std::cout << "Error: " << e.what();
        }
    }
}

// Splits a string into a vector of substrings based on the specified delimiter (default is space).
std::vector<std::string> split(const std::string &str, char delimiter = ' ') {
    std::vector<std::string> words;
    std::stringstream ss(str);
    std::string word;
    while (getline(ss, word, delimiter)) {
        words.push_back(word);
    }
    return words;
}

// Prompts the user to enter a username and verifies that it follows a specific format.
bool getNameFromUser(std::string &name) {
    std::cout << "Enter your username: ";
    name = getLine();

    std::regex pattern("^[^\\d]\\w+$");
    if (!std::regex_match(name, pattern)) {
        std::cout << "Invalid username format." << std::endl;
        return false;
    }
    return true;
}

#endif //UTILS_H
