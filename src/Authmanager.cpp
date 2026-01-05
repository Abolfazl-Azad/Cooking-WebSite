
#include "AuthManager.hpp"

AuthManager::AuthManager() : currentUser(nullptr) {}

AuthManager::~AuthManager() {
    for (auto user : users) {
        delete user;
    }
}

User* AuthManager::findUser(string username) {
    for (auto u : users) {
        if (u->getUsername() == username)
            return u;
    }
    return nullptr;
}

string AuthManager::signup(string role, string username, string password) {
    if (currentUser != nullptr) {
        return Messages::PERMISSION_DENIED;
    }
    
    if (findUser(username) != nullptr) {
        return Messages::BAD_REQUEST;
    }
    
    User* newUser = nullptr;
    if (role == Roles::CHEF) {
        newUser = new Chef(username, password);
    } else if (role == Roles::VISITOR) {
        newUser = new Visitor(username, password);
    } else {
        return Messages::BAD_REQUEST;
    }
    
    users.push_back(newUser);
    currentUser = newUser;
    return Messages::OK;
}

string AuthManager::login(string username, string password) {
    if (currentUser != nullptr) {
        return Messages::PERMISSION_DENIED;
    }

    
    
    User* user = findUser(username);
    if(user == nullptr){
        return Messages::NOT_FOUND;
    }
    if(user -> getPassword() != password){
        return Messages::PERMISSION_DENIED;
    }
    
    currentUser = user;
    return Messages::OK;
}

string AuthManager::logout() {
    if (currentUser == nullptr) {
        return Messages::PERMISSION_DENIED;
    }
    
    currentUser = nullptr;
    return Messages::OK;
}

void AuthManager::setCurrentUser(User* user) {
    currentUser = user;
}
