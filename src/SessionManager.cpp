#include "SessionManager.hpp"

SessionManager::SessionManager() : nextId(0) {}

std::string SessionManager::createSession(User* user) {
    if (user == nullptr) {
        return "";
    }
    ++nextId;
    std::string id = std::to_string(nextId);
    sessions[id] = user;
    return id;
}

User* SessionManager::getUser(const std::string& sessionId) const {
    auto it = sessions.find(sessionId);
    if (it == sessions.end()) {
        return nullptr;
    }
    return it->second;
}

void SessionManager::removeSession(const std::string& sessionId) {
    sessions.erase(sessionId);
}
