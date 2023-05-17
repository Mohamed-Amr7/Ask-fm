
# C++ OOP Project - Question & Answer Forum
This is a C++ object-oriented programming project implementing a basic question and answer forum. It has the following features:

## Features
### Account Sign Up and Login
Users can sign up for a new account by providing a username, password, and email
Existing users can log in by providing their username and password

### Posting Questions
Logged-in users can post new questions.
Questions have a title, description, and are posted under a specific topic or category

### Posting Answers
Logged-in users can post answers to existing questions or threads.

![Feed](/assets/Feed.png)

### Deleting Content
Logged-in users can delete questions or answers they posted
Users can also delete their own account which will delete all their questions.

### Listing System Users
Users can view all users on the system with their IDs and usernames
![Listing System Users](/assets/Users.png)

### Object Oriented Design
This project utilizes an object-oriented design with the following classes:
- User: Represents a user account with username, password and ID
- Question: Represents a question with a description,ID and answers
- Thread: Represents a thread on a question with a description and answer

