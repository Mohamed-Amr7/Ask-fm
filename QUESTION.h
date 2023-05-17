#ifndef QUESTION_H
#define QUESTION_H

#include "THREAD.h"
#include <vector>

class Question : public Thread {

private:
    int QuestionID_;
    static int numberOfQuestions;
public:
    std::vector<Thread> threads;
    // A static vector containing all the questions in the program.
    static std::vector<Question> allQuestions;

    Question(int authorID, int recipientID, std::string questionText, int QuestionID, bool isAuthorAnon)
            : Thread(authorID, recipientID, std::move(questionText), isAuthorAnon) {
        QuestionID_ = QuestionID;
        numberOfQuestions++;
        allQuestions.push_back(*this);
    }

    // Getters
    int getQuestionID() const {
        return QuestionID_;
    }

    static int getNumberOfQuestions() {
        return numberOfQuestions;
    }


    // These 2 functions cannot be merged due to different return types and purposes.

    // Validate if there is a question with this ID
    static bool isQuestionValid(const int &questionID) {
        int left = 0;
        int right = (int) Question::allQuestions.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (Question::allQuestions[mid].getQuestionID() == questionID) {
                return true;
            }
            else if (Question::allQuestions[mid].getQuestionID() < questionID) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        return false;
    }

    // Returns the Question object with the specified ID
    static Question &getQuestion(const int &questionID) {
        int left = 0;
        int right = (int) Question::allQuestions.size() - 1;
        int idx = 0;
        // Binary search for a question with the given ID in allQuestions vector
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (Question::allQuestions[mid].getQuestionID() == questionID) {
                idx = mid;
                break;
            }
            else if (Question::allQuestions[mid].getQuestionID() < questionID) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        return Question::allQuestions[idx];
    }

    // Returns a valid question ID, or -1 if the user cancels.
    static int getQuestionIdFromUser() {
        int questionID = getInt();
        while (questionID != -1 and !Question::isQuestionValid(questionID)) {
            std::cout << "Question ID is not Valid. Please enter a valid question ID: ";
            questionID = getInt();
        }
        return questionID;
    }

    // Displays a question and prompts the user to select a thread to answer (if applicable).
    int promptThreadSelection() const {
        int numOfThreads = (int) this->threads.size();
        int threadId{-1};

        if (numOfThreads >= 1) {
            if (numOfThreads == 1) std::cout << "Enter 0 to answer the thread or -1 to answer the question: ";

            else
                std::cout << "Select the thread you want to answer (0 - "
                          << numOfThreads - 1 << ") or -1 to answer the question: ";

            threadId = getInt(-1, numOfThreads - 1);
        }

        return threadId;
    }

    // Prompts the user to enter an answer for the given question or thread.
    void promptAnswer(int threadId) {
        std::cout << "Enter answer text: ";
        std::string answerText = getLine();

        // Set the answer to the question or thread based on user input
        // and notify the user if the question is already answered
        if (threadId == -1) {
            if (!this->getAnswer().empty()) {
                std::cout << "Warning: Question already answered." << std::endl
                          << "Enter -1 to cancel or 0 to override the current answer";
                int option = getInt(-1, 0);
                if (option == -1) return;
            }
            this->setAnswer(answerText);
        }
        else {
            if (!this->threads[threadId].getAnswer().empty()) {
                std::cout << "Warning: Thread already answered." << std::endl
                          << "Enter -1 to cancel or 0 to override the current answer";
                int option = getInt(-1, 0);
                if (option == -1) return;
            }
            this->threads[threadId].setAnswer(answerText);
        }
    }

    // Overloading comparison operators
    bool operator==(const Question &other) const {
        return this->getQuestionID() == other.getQuestionID();
    }

    bool operator!=(const Question &other) const {
        return this->getQuestionID() != other.getQuestionID();
    }

    bool operator>(const Question &other) const {
        return this->getQuestionID() == other.getQuestionID();
    }

    bool operator<(const Question &other) const {
        return this->getQuestionID() == other.getQuestionID();
    }

    // Overloading insertion operator to print a User object to the output stream
    friend std::ostream &operator<<(std::ostream &os, const Question &quest) {
        os << "Question Id(" << quest.getQuestionID() << ") ";

        // If the author is not anonymous print his ID
        if (!quest.getIsAuthorAnon()) {
            os << "from user(" << quest.getAuthorID() << ") ";
        }
        // print the recipient ID and the question itself
        os << "to user(" << quest.getRecipientId() << ") \n";
        os << "\tQuestion: " << quest.getQuestionText() << std::endl;

        // If the question has an answer print it
        if (!quest.getAnswer().empty()) {
            os << "\tAnswer: " << quest.getAnswer() << std::endl;
        }
        os << std::endl;
        // Print all the child threads of this question
        for (const auto &thread: quest.threads) {
            std::cout << thread;
        }
        return os;
    }
};

#endif //QUESTION_H
