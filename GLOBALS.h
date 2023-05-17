#ifndef GLOBALS_H
#define GLOBALS_H

#include "QUESTION.h"
#include "USER.h"
#include "unordered_map"

std::vector<Question> Question::allQuestions;

int Question::numberOfQuestions{0};

std::unordered_map<std::string, bool> User::usernameStatus;

std::unordered_map<int, bool> User::idStatus;

std::unordered_map<std::string, int> User::userInfo;

std::vector<User> User::allUsers;

int User::numberOfUsers{0};

#endif //GLOBALS_H