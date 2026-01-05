#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <string>
#include <unordered_map>

#include "User.hpp"

class SessionManager {
public:
    SessionManager();

    std::string createSession(User* user);
    User* getUser(const std::string& sessionId) const;
    void removeSession(const std::string& sessionId);

private:
    std::unordered_map<std::string, User*> sessions;
    int nextId;
};

#endif
