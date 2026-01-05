#ifndef WEB_VIEW_HPP
#define WEB_VIEW_HPP

#include <string>

#include "User.hpp"

namespace WebView {

std::string escape(const std::string& value);
std::string layout(const std::string& title,
                   const std::string& body,
                   const User* user,
                   const std::string& pageKey,
                   const std::string& message,
                   const std::string& messageType);

} 

#endif
