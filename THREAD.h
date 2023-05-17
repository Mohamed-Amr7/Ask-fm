#ifndef THREAD_H
#define THREAD_H

class Thread {
protected:
    int authorID_;
    std::string answer_;
    std::string questionText_;
    bool isAuthorAnon_;
    int recipientID_;

public:
    Thread(int authorID, int recipientID, std::string questionText, bool isAuthorAnon) {
        authorID_ = authorID;
        recipientID_ = recipientID;
        questionText_ = std::move(questionText);
        isAuthorAnon_ = isAuthorAnon;
    }

    bool getIsAuthorAnon() const {
        return isAuthorAnon_;
    }

    std::string getAnswer() const {
        return answer_;
    }

    std::string getQuestionText() const {
        return questionText_;
    }

    int getAuthorID() const {
        return authorID_;
    }

    int getRecipientId() const {
        return recipientID_;
    }

    void setAnswer(std::string answer) {
        answer_ = std::move(answer);
    }

    friend std::ostream &operator<<(std::ostream &os, const Thread &thread) {
        os << "\tThread ";
        // If the author is not anonymous print his ID
        if (!thread.getIsAuthorAnon()) {
            os << "from user(" << thread.getAuthorID() << ") ";
        }
        // print the recipient ID and the question itself
        os << "to user(" << thread.getRecipientId() << "):\n";
        os << "\t\tQuestion: " << thread.getQuestionText() << std::endl;

        // If the question has an answer print it
        if (!thread.getAnswer().empty()) {
            os << "\t\tAnswer: " << thread.getAnswer() << std::endl;
        }
        os << std::endl;
        return os;
    }
};

#endif //THREAD_H
