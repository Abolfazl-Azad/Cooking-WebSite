
#ifndef AUTH_MANAGER_HPP
#define AUTH_MANAGER_HPP

#include "global.hpp"
#include "User.hpp"

class AuthManager {
private:
    vector<User*> users;
    User* currentUser;
    
    User* findUser(string username);

public:
    AuthManager();
    ~AuthManager();
    
    string signup(string role, string username, string password);
    string login(string username, string password);
    string logout();
    
    User* getCurrentUser() const { return currentUser; }
    bool isLoggedIn() const { return currentUser != nullptr; }
    void setCurrentUser(User* user);
};

#endif
