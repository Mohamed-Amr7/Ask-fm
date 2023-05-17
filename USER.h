#ifndef USER_H
#define USER_H

#include <vector>
#include <iostream>
#include <unordered_map>
#include "UTILS.h"
#include "QUESTION.h"

const int questionIdDiff = 100;

class User {

private:
    std::string username_;
    std::string password_;
    bool allowAnonymousQuestions_;
    int ID_;
    static int numberOfUsers;

public:
    std::vector<int> questionsSentID;
    std::vector<int> questionsReceivedID;
    static std::unordered_map<std::string, bool> usernameStatus;
    static std::unordered_map<int, bool> idStatus;
    static std::unordered_map<std::string, int> userInfo;
    static std::vector<User> allUsers;

    // User constructor
    User(std::string username, std::string password, bool allowAnonymousQuestions, int ID) {
        ++numberOfUsers;
        username_ = username;
        password_ = password;
        allowAnonymousQuestions_ = allowAnonymousQuestions;
        ID_ = ID;
    }

    // Getters
    std::string getName() const {
        return username_;
    }

    std::string getPassword() const {
        return password_;
    }

    int getID() const {
        return ID_;
    }

    bool getIsAnonAllowed() const {
        return allowAnonymousQuestions_;
    };

    static int getNumberOfUsers() {
        return numberOfUsers;
    }
    // Validate if there is a user with these credentials
    static bool isUserValid(const std::string &username) {
        int userId = User::userInfo[username];
        int left = 0;
        int right = (int) User::allUsers.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (User::allUsers[mid].getID() == userId) {
                return true;
            } else if (User::allUsers[mid].getID() < userId) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return false;
    }

    // Returns the User object with the specified ID
    static User &getUser(const int &userId) {
        int left = 0;
        int right = (int) User::allUsers.size() - 1;
        int idx = 0;
        // Binary search for a User with the given ID in allUsers vector
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (User::allUsers[mid].getID() == userId) {
                idx = mid;
                break;
            } else if (User::allUsers[mid].getID() < userId) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return User::allUsers[idx];
    }

    // Returns the User object with the specified username
    static User &getUser(const std::string &username) {
        int id = userInfo[username];
        return getUser(id);
    }

    void askQuestion() {
        std::cout << "Enter User ID or -1 to cancel: ";
        int recipientId = getInt();

        // Verify that there is a user with that ID
        while (recipientId != -1 and User::idStatus[recipientId] == 0) {
            std::cout << "User ID is not Valid. Please enter a valid ID or -1 to cancel: ";
            recipientId = getInt();
        }
        if (recipientId == -1) return;

        User &recipient = User::getUser(recipientId);

        // Check if the recipient allows anonymous questions
        bool askAnonymously;
        if (recipient.getIsAnonAllowed()) {
            std::cout << "Do you want to ask the question anonymously (0/1): ";
            askAnonymously = getInt(0, 1);
        }
        else std::cout << "Note: Anonymous questions are not allowed for this user" << std::endl;

        // Check if the user wants to ask a thread or a new question
        std::cout << "For thread question: Enter Question id or -1 for a new question: ";

        int questionID = Question::getQuestionIdFromUser();

        bool isThread = false;
        if (questionID != -1) {
            isThread = true;
        }
        // Question IDs start from 100
        else {
            questionID = Question::getNumberOfQuestions() + questionIdDiff;
        }

        std::cout << "Enter Question text: ";
        std::string questionText;
        questionText = getLine();

        // Add thread to the parent question
        if (isThread) {
            Question question = Question::getQuestion(questionID);
            question.threads.emplace_back(
                    this->getID(), recipient.getID(), questionText, askAnonymously);
        }
        // Create a new question and assign it to the author and recipient, then
        // save the question to the collection of all questions.
        else {
            // Add question ID to the User (author) object
            this->questionsSentID.push_back(questionID);
            // If the author is the recipient add the question ID to the received question vector
            if (this->getID() == recipientId) {
                this->questionsReceivedID.push_back(questionID);
                auto itr = std::lower_bound(allUsers.begin(), allUsers.end(), *this);
                int index = std::distance(allUsers.begin(), itr);
                allUsers[index] = *this;
            }
            // Get the Recipient object and add the question to the received question vector
            else {
                recipient.questionsReceivedID.push_back(questionID);
                auto itr = std::lower_bound(allUsers.begin(), allUsers.end(), *this);
                int index = std::distance(allUsers.begin(), itr);
                allUsers[index] = *this;
                itr = std::lower_bound(allUsers.begin(), allUsers.end(), recipient);
                index = std::distance(allUsers.begin(), itr);
                allUsers[index] = recipient;
            }
            Question(this->getID(), recipient.getID(),
                     questionText, questionID, askAnonymously);
        }
    }

    // Allows the user to answer a question or thread by providing a question ID and answer text.
    void answerQuestion(const int questionId) {
        // Get the question object corresponding to the given ID
        Question &question = Question::getQuestion(questionId);
        std::cout << question;

        // Prompt the user to select a thread to answer (if applicable)
        int threadId = question.promptThreadSelection();

        // Prompts the user for an answer and sets it to a thread or question based on user input.
        // If the user enters -1, the answer is set to the question; otherwise, it's set to the selected thread.
        question.promptAnswer(threadId);
    }

    // Allows the user to delete a question by providing a question ID
    void deleteQuestion(const int questionId) {
        // Getting the question , author ID and recipient ID
        Question question = Question::getQuestion(questionId);
        int authorId = question.getAuthorID();
        int recipientId = question.getRecipientId();

        // Getting the author and recipient from their IDs
        User author = getUser(authorId);
        User recipient = getUser(recipientId);

        // Deleting the question by binary search
        auto foundQuestionIdItr = lower_bound(author.questionsSentID.begin(), author.questionsSentID.end(), questionId);
        author.questionsSentID.erase(foundQuestionIdItr);

        foundQuestionIdItr = lower_bound(recipient.questionsReceivedID.begin(), recipient.questionsReceivedID.end(),
                                         questionId);
        recipient.questionsReceivedID.erase(foundQuestionIdItr);

        auto foundQuestionItr = std::lower_bound(Question::allQuestions.begin(), Question::allQuestions.end(),
                                                 question);
        Question::allQuestions.erase(foundQuestionItr);
    }

    void deleteUserAccount() {
        int id = this->getID();
        std::string username = this->getName();
        for (auto &question: User::questionsSentID) {
            deleteQuestion(question);
        }
        // Set usernameStatus and idStatus to false to indicate that they are free
        usernameStatus[username] = false;
        idStatus[id] = false;
        userInfo.erase("username");
    }

    // Prompts the user to confirm deletion of their account and deletes it if confirmed with correct password
    void deleteUserAccountPrompt() {
        std::cout
                << "Are you sure you want to delete your account? This action is irreversible and all your data will be lost. (0/1)"
                << std::endl;
        int option;
        option = getInt(0, 1);
        if (option) {
            std::cout << "Please enter your password to confirm your identity: ";
            std::string password;
            password = getLine();
            if (this->getPassword() == password) {
                std::cout << "Your account has been deleted." << std::endl;
                this->deleteUserAccount();
            }
            else {
                std::cout << "Incorrect password. Your account has not been deleted." << std::endl;
            }
        }
        else {
            std::cout << "Your account has not been deleted." << std::endl;
        }

    }

    // Questions printing methods
    void printQuestionsToMe() {
        for (int &questionID: questionsReceivedID) {
            auto question = Question::allQuestions[questionID - questionIdDiff];
            std::cout << "Question Id(" << questionID << ") ";
            // If the author is not anonymous print his ID

            if (!question.getIsAuthorAnon()) {
                std::cout << "from user(" << question.getAuthorID() << ") ";
            }
            // Print the question itself
            std::cout << "\n\tQuestion: " << question.getQuestionText() << std::endl;
            // If the question has an answer print it
            if (!question.getAnswer().empty()) {
                std::cout << "\tAnswer: " << question.getAnswer() << std::endl;
            }
            std::cout << std::endl;
            // Print the child threads of this question
            for (const auto &thread: question.threads) {
                std::cout << thread;
            }
        }
    }

    void printQuestionsFromMe() {
        for (int &questionID: questionsSentID) {
            auto question = Question::allQuestions[questionID - questionIdDiff];
            // Print the question ID and recipient ID
            std::cout << "Question Id(" << questionID << ") ";
            std::cout << "to user(" << question.getRecipientId() << ") \n";

            // print the question itself
            std::cout << "\tQuestion: " << question.getQuestionText() << std::endl;
            // If the question has an answer print it
            if (!question.getAnswer().empty()) {
                std::cout << "\tAnswer: " << question.getAnswer() << std::endl;
            }
            std::cout << std::endl;
            // Print the child threads of this question
            for (const auto &thread: question.threads) {
                std::cout << thread;
            }
        }
    }

    // Overloading comparison operators
    bool operator==(const User &other) const {
        return this->getID() == other.getID();
    }

    bool operator!=(const User &other) const {
        return this->getID() != other.getID();
    }

    bool operator>(const User &other) const {
        return this->getID() > other.getID();
    }

    bool operator<(const User &other) const {
        return this->getID() < other.getID();
    }

    // Overloading insertion operator to print a User object to the output stream
    friend std::ostream &operator<<(std::ostream &os, const User &user) {
        os << "ID: " << user.getID() << "\tName: " << user.getName() << std::endl;
        return os;
    }

};

#endif //USER_H
