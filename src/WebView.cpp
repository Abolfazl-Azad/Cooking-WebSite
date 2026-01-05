#include "WebView.hpp"

#include <sstream>

#include "global.hpp"

namespace {

std::string navLinks(const User* user) {
    std::ostringstream out;
    out << "<nav class=\"nav\">";
    out << "<a href=\"/home\">Home</a>";

    if (user == nullptr) {
        out << "<a href=\"/login\">Login</a>";
        out << "<a href=\"/signup\">Sign up</a>";
        out << "</nav>";
        return out.str();
    }

    if (user->getRole() == CHEF) {
        out << "<a href=\"/chef/recipe\">Add recipe</a>";
        out << "<a href=\"/chef/most-liked\">Most liked</a>";
    } else if (user->getRole() == VISITOR) {
        out << "<a href=\"/visitor/fridge\">Add ingredient</a>";
        out << "<a href=\"/visitor/recommendations\">Recommendations</a>";
        out << "<a href=\"/visitor/like\">Like recipe</a>";
    }

    out << "<a href=\"/logout\" class=\"danger\">Logout</a>";
    out << "</nav>";
    return out.str();
}

std::string roleLabel(const User* user) {
    if (user == nullptr) {
        return "Guest";
    }
    return user->getRole() == CHEF ? "Chef" : "Visitor";
}

} 

namespace WebView {

std::string escape(const std::string& value) {
    std::string out;
    out.reserve(value.size());
    for (char ch : value) {
        switch (ch) {
        case '&': out += "&amp;"; break;
        case '<': out += "&lt;"; break;
        case '>': out += "&gt;"; break;
        case '"': out += "&quot;"; break;
        default: out += ch; break;
        }
    }
    return out;
}

std::string layout(const std::string& title,
                   const std::string& body,
                   const User* user,
                   const std::string& pageKey,
                   const std::string& message,
                   const std::string& messageType) {
    std::ostringstream html;
    html << "<!DOCTYPE html>";
    html << "<html lang=\"en\">";
    html << "<head>";
    html << "<meta charset=\"UTF-8\" />";
    html << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />";
    html << "<title>UTCook | " << escape(title) << "</title>";
    html << "<link rel=\"stylesheet\" href=\"/static/style.css\" />";
    html << "<script defer src=\"/static/app.js\"></script>";
    html << "</head>";

    html << "<body data-page=\"" << escape(pageKey) << "\" data-role=\"" << escape(roleLabel(user)) << "\">";
    html << "<header class=\"topbar\">";
    html << "<div class=\"brand\">";
    html << "<div class=\"brand-mark\"></div>";
    html << "<div>";
    html << "<div class=\"brand-title\">UTCook</div>";
    html << "<div class=\"brand-sub\">Smart kitchen companion</div>";
    html << "</div>";
    html << "</div>";
    html << navLinks(user);
    if (user != nullptr) {
        html << "<div class=\"user-chip\">" << escape(user->getUsername()) << " · " << escape(roleLabel(user)) << "</div>";
    }
    html << "</header>";

    html << "<main class=\"content\">";
    html << "<section class=\"card hero\">";
    html << "<h1>" << escape(title) << "</h1>";
    html << "<p>Manage your kitchen with confidence.</p>";
    html << "</section>";

    if (!message.empty()) {
        html << "<section class=\"card flash flash-" << escape(messageType) << "\">";
        html << "<p>" << escape(message) << "</p>";
        html << "</section>";
    }

    html << body;
    html << "</main>";
    html << "</body></html>";
    return html.str();
}

} 
