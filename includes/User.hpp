#ifndef USER_HPP
#define USER_HPP

#include "global.hpp"

enum UserRole { CHEF, VISITOR };

class User {
protected:
    string username;
    string password;
    UserRole role;
    map<string, int> shelf;

public:
    User(string user, string pass, UserRole r) 
        : username(user), password(pass), role(r) {}

    void addToShelf(string ingName, int amount) {
        shelf[ingName] += amount;
    }

    map<string, int> getShelf() const { return shelf; }

    string getUsername() const { return username; }
    string getPassword() const { return password; }
    UserRole getRole() const { return role; }

    virtual ~User() = default; 
};

class Chef : public User {
public:
    Chef(string user, string pass) : User(user, pass, CHEF) {}
};

class Visitor : public User {
public:
    Visitor(string user, string pass) : User(user, pass, VISITOR) {}
};

#endif