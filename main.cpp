#include <bits/stdc++.h>
#include "UTILS.h"
#include "GLOBALS.h"
#include "QUESTION.h"

// Prints a list of all system users.
void listSystemUsers() {
    for (const auto &user: User::allUsers) {
        std::cout << user;
    }
}

// Prints all the questions in the feed.
void printFeed() {
    for (const auto &question: Question::allQuestions) {
        std::cout << question;
    }
}

// Returns the User with these credentials
User &login() {
    std::string username;
    std::string password;

    std::cout << "Enter username and password: ";
    std::cin >> username >> password;

    // keep prompting user for username and password until they are valid
    while (!User::isUserValid(username)) {
        std::cout << "Invalid credentials. Please enter your username and password again or -1 to cancel : ";
        std::string inputStr = getLine();
        std::vector<std::string> inputList = split(inputStr);

        username = inputList[0];
        if (inputList.size() > 1) password = inputList[1];
        if (inputList.size() > 2) {
            username.clear();
            password.clear();
            continue;
        }
    }

    return User::getUser(username);
}

void signUp() {

    // Get username
    std::string name;
    std::cout << "Enter username. (No spaces): ";
    while (getNameFromUser(name) and User::usernameStatus[name]) {
        std::cout << "This username is already taken. Please try again: ";
        std::cin >> name;
        while (!isalpha(name[0])) {
            std::cout << "Username should start with a letter Please try again:";
            std::cin >> name;
        }
    }
    // Get password
    std::cout << "Enter password: ";
    std::cin >> std::ws;
    std::string password = getLine();

    // Allow anonymous questions?
    std::cout << "Allow anonymous questions? (0/1): ";
    bool allowAnon = getInt(0, 1); // allow anonymous questions
    int id = User::getNumberOfUsers() + 1;
    User::allUsers.emplace_back(name, password, allowAnon, id);

    // Set the username and ID status to true to indicate that they are in use
    User::usernameStatus[name] = true;
    User::idStatus[id] = true;
    User::userInfo[name] = id;
}

int loginMenu() {

    std::cout << "Menu:\n"
              << "\t1: Login\n"
              << "\t2: Sign Up\n"
              << "\t3: Exit\n" << std::endl
              << "Enter a number in range 1 - 3:  ";

    int option = getInt(1, 3);
    std::cout << std::endl;
    return option;
}

void mainMenu() {
    std::cout << "\nMenu:\n"
              << "\t1: Print Questions to Me\n"
              << "\t2: Print Questions from Me\n"
              << "\t3: Answer Question\n"
              << "\t4: Delete Question\n"
              << "\t5: Ask Question\n"
              << "\t6: List System Users\n"
              << "\t7: Feed\n"
              << "\t8: Logout\n"
              << "\t9: Delete My Account\n"
              << "\nEnter a number in range 1 - 9: " << std::flush;
}

void ask() {
    while (true) {
        int option = loginMenu();
        if (option == 1)
        {
            User currUser = login();
            while (true)
            {
                mainMenu();
                option = getInt(1, 9);
                if (option == 1){
                    currUser.printQuestionsToMe();
                }
                else if (option == 2){
                    currUser.printQuestionsFromMe();
                }
                else if (option == 3) {
                    std::cout << "Enter question ID or -1 to cancel: ";
                    int questionId;
                    questionId = Question::getQuestionIdFromUser();
                    currUser.answerQuestion(questionId);
                }
                else if (option == 4) {
                    std::cout << "Enter question ID or -1 to cancel: ";
                    int questionId;
                    questionId = Question::getQuestionIdFromUser();
                    currUser.deleteQuestion(questionId);
                }
                else if (option == 5) {
                    currUser.askQuestion();
                }
                else if (option == 6) {
                    listSystemUsers();
                }
                else if (option == 7) {
                    printFeed();
                }
                else if (option == 8) {
                    break;
                }
                else if (option == 9) {
                    currUser.deleteUserAccountPrompt();
                }
            }
        } else if (option == 2)
            signUp();
        else if (option == 3)
            break;

    }
}

int main() {
    ask();
    return 0;
}