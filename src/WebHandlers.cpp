#include "WebHandlers.hpp"

#include <sstream>

#include "WebView.hpp"

namespace {

std::string buildLoginBody() {
    std::ostringstream body;
    body << "<section class=\"card form-card\">";
    body << "<form method=\"post\" action=\"/login\" class=\"form\">";
    body << "<label>Username</label>";
    body << "<input name=\"username\" required autocomplete=\"username\" />";
    body << "<label>Password</label>";
    body << "<input type=\"password\" name=\"password\" required autocomplete=\"current-password\" />";
    body << "<button type=\"submit\" class=\"primary\">Login</button>";
    body << "</form>";
    body << "<p class=\"helper\">No account yet? <a href=\"/signup\">Create one</a>.</p>";
    body << "</section>";
    return body.str();
}

std::string buildSignupBody() {
    std::ostringstream body;
    body << "<section class=\"card form-card\">";
    body << "<form method=\"post\" action=\"/signup\" class=\"form\">";
    body << "<label>Role</label>";
    body << "<select name=\"role\" required>";
    body << "<option value=\"chef\">Chef</option>";
    body << "<option value=\"visitor\">Visitor</option>";
    body << "</select>";
    body << "<label>Username</label>";
    body << "<input name=\"username\" required autocomplete=\"username\" />";
    body << "<label>Password</label>";
    body << "<input type=\"password\" name=\"password\" required autocomplete=\"new-password\" />";
    body << "<button type=\"submit\" class=\"primary\">Sign up</button>";
    body << "</form>";
    body << "<p class=\"helper\">Already have an account? <a href=\"/login\">Login</a>.</p>";
    body << "</section>";
    return body.str();
}

std::string buildHomeBody(User* user) {
    std::ostringstream body;
    body << "<section class=\"grid\">";
    if (user->getRole() == CHEF) {
        body << "<article class=\"card\">";
        body << "<h2>Add a new recipe</h2>";
        body << "<p>Create dishes and share steps with visitors.</p>";
        body << "<a class=\"button\" href=\"/chef/recipe\">Open</a>";
        body << "</article>";
        body << "<article class=\"card\">";
        body << "<h2>Most liked report</h2>";
        body << "<p>See which recipes are trending in your kitchen.</p>";
        body << "<a class=\"button\" href=\"/chef/most-liked\">Open</a>";
        body << "</article>";
    } else {
        body << "<article class=\"card\">";
        body << "<h2>Add ingredients</h2>";
        body << "<p>Update your fridge and keep it accurate.</p>";
        body << "<a class=\"button\" href=\"/visitor/fridge\">Open</a>";
        body << "</article>";
        body << "<article class=\"card\">";
        body << "<h2>Get recommendations</h2>";
        body << "<p>Let the recommenders find your next meal.</p>";
        body << "<a class=\"button\" href=\"/visitor/recommendations\">Open</a>";
        body << "</article>";
        body << "<article class=\"card\">";
        body << "<h2>Like recipes</h2>";
        body << "<p>Support your favorite dishes.</p>";
        body << "<a class=\"button\" href=\"/visitor/like\">Open</a>";
        body << "</article>";
    }
    body << "</section>";
    return body.str();
}

std::string buildRecipeFormBody() {
    std::ostringstream body;
    body << "<section class=\"card form-card\">";
    body << "<form method=\"post\" action=\"/chef/recipe\" class=\"form\" data-confirm=\"add recipe\">";
    body << "<label>Recipe title</label>";
    body << "<input name=\"title\" required placeholder=\"Sunny Breakfast\" />";
    body << "<label>Ingredients (name:amount;name:amount)</label>";
    body << "<textarea name=\"ingredients\" rows=\"4\" required ";
    body << "placeholder=\"eggs:2;tomato:1;butter:1\"></textarea>";
    body << "<label>Steps (separate with semicolons)</label>";
    body << "<textarea name=\"steps\" rows=\"4\" required ";
    body << "placeholder=\"Beat eggs;Heat pan;Cook until set\"></textarea>";
    body << "<button type=\"submit\" class=\"primary\">Save recipe</button>";
    body << "</form>";
    body << "</section>";
    return body.str();
}

std::string buildIngredientFormBody() {
    std::ostringstream body;
    body << "<section class=\"card form-card\">";
    body << "<form method=\"post\" action=\"/visitor/fridge\" class=\"form\" data-confirm=\"update fridge\">";
    body << "<label>Ingredient name</label>";
    body << "<input name=\"name\" required placeholder=\"eggs\" />";
    body << "<label>Amount</label>";
    body << "<input type=\"number\" name=\"amount\" min=\"1\" required />";
    body << "<button type=\"submit\" class=\"primary\">Update fridge</button>";
    body << "</form>";
    body << "</section>";
    return body.str();
}

std::string buildRecommendationsBody(const std::vector<std::string>& recommenders,
                                     const std::string& selected,
                                     const std::string& resultHtml) {
    std::ostringstream body;
    body << "<section class=\"card form-card\">";
    body << "<form method=\"post\" action=\"/visitor/recommendations\" class=\"form\">";
    body << "<label>Select recommender</label>";
    body << "<select name=\"name\" required>";
    for (const auto& name : recommenders) {
        body << "<option value=\"" << WebView::escape(name) << "\"";
        if (name == selected) {
            body << " selected";
        }
        body << ">" << WebView::escape(name) << "</option>";
    }
    body << "</select>";
    body << "<button type=\"submit\" class=\"primary\">Get suggestions</button>";
    body << "</form>";
    body << "</section>";
    if (!resultHtml.empty()) {
        body << "<section class=\"card\">";
        body << "<h2>Results</h2>";
        body << resultHtml;
        body << "</section>";
    }
    return body.str();
}

std::string buildLikeBody(const std::vector<Recipe>& recipes) {
    std::ostringstream body;
    body << "<section class=\"card form-card\">";
    body << "<form method=\"post\" action=\"/visitor/like\" class=\"form\">";
    body << "<label>Pick a recipe</label>";
    body << "<select name=\"recipe\" required>";
    for (const auto& recipe : recipes) {
        body << "<option value=\"" << WebView::escape(recipe.getName()) << "\">";
        body << WebView::escape(recipe.getName()) << "</option>";
    }
    body << "</select>";
    body << "<button type=\"submit\" class=\"primary\">Like</button>";
    body << "</form>";
    body << "</section>";
    return body.str();
}

std::string buildMostLikedBody(const std::string& countValue, const std::string& resultHtml) {
    std::ostringstream body;
    body << "<section class=\"card form-card\">";
    body << "<form method=\"post\" action=\"/chef/most-liked\" class=\"form\">";
    body << "<label>How many recipes?</label>";
    body << "<input type=\"number\" name=\"count\" min=\"1\" value=\"" << WebView::escape(countValue) << "\" />";
    body << "<button type=\"submit\" class=\"primary\">Generate report</button>";
    body << "</form>";
    body << "</section>";
    if (!resultHtml.empty()) {
        body << "<section class=\"card\">";
        body << "<h2>Report</h2>";
        body << resultHtml;
        body << "</section>";
    }
    return body.str();
}

} 

BaseWebHandler::BaseWebHandler(WebApp& app, SessionManager& sessions)
    : app(app), sessions(sessions) {}

User* BaseWebHandler::resolveUser(Request* req) {
    User* user = sessions.getUser(req->getSessionId());
    app.auth().setCurrentUser(user);
    return user;
}

Response* BaseWebHandler::htmlResponse(const std::string& html) {
    Response* res = new Response();
    res->setHeader("Content-Type", "text/html");
    res->setBody(html);
    return res;
}

Response* BaseWebHandler::redirectTo(const std::string& url, const std::string& sessionId) {
    Response *res = Response::redirect(url);
    if (!sessionId.empty()) {
        res->setSessionId(sessionId);
    }
    return res;
}

Response* BaseWebHandler::errorPage(const std::string& message, User* user) {
    std::string html = WebView::layout("Action failed", "<section class=\"card\"></section>",
                                       user, "error", message, "error");
    return htmlResponse(html);
}

std::string BaseWebHandler::humanMessage(const std::string& result) const {
    if (result == Messages::BAD_REQUEST) {
        return "Bad request. Please check the form values.";
    }
    if (result == Messages::NOT_FOUND) {
        return "Requested resource was not found.";
    }
    if (result == Messages::PERMISSION_DENIED) {
        return "You do not have permission to perform this action.";
    }
    if (result == Messages::ALREADY_EXISTS) {
        return "This item already exists.";
    }
    if (result == Messages::EMPTY) {
        return "No data to show yet.";
    }
    if (result == Messages::NO_ITEMS_IN_FRIDGE) {
        return "No item exists in the refrigerator.";
    }
    if (result == Messages::CHEF_HAS_NO_RECIPE) {
        return "You have not added any recipes yet.";
    }
    return "Something went wrong.";
}

RootHandler::RootHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* RootHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user == nullptr) {
        return redirectTo("/login");
    }
    return redirectTo("/home");
}

LoginPageHandler::LoginPageHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* LoginPageHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user != nullptr) {
        return redirectTo("/home");
    }
    std::string html = WebView::layout("Login", buildLoginBody(), nullptr, "login", "", "info");
    return htmlResponse(html);
}

LoginHandler::LoginHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* LoginHandler::callback(Request* req) {
    User* current = resolveUser(req);
    if (current != nullptr) {
        return redirectTo("/home");
    }

    std::string username = req->getBodyParam("username");
    std::string password = req->getBodyParam("password");
    std::string result = app.auth().login(username, password);

    if (result == Messages::OK) {
        User* user = app.auth().getCurrentUser();
        std::string sessionId = sessions.createSession(user);
        return redirectTo("/home", sessionId);
    }

    std::string html = WebView::layout("Login", buildLoginBody(), nullptr,
                                       "login", humanMessage(result), "error");
    return htmlResponse(html);
}

SignupPageHandler::SignupPageHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* SignupPageHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user != nullptr) {
        return redirectTo("/home");
    }
    std::string html = WebView::layout("Sign up", buildSignupBody(), nullptr, "signup", "", "info");
    return htmlResponse(html);
}

SignupHandler::SignupHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* SignupHandler::callback(Request* req) {
    User* current = resolveUser(req);
    if (current != nullptr) {
        return redirectTo("/home");
    }

    std::string role = req->getBodyParam("role");
    std::string username = req->getBodyParam("username");
    std::string password = req->getBodyParam("password");
    std::string result = app.auth().signup(role, username, password);

    if (result == Messages::OK) {
        User* user = app.auth().getCurrentUser();
        std::string sessionId = sessions.createSession(user);
        return redirectTo("/home", sessionId);
    }

    std::string html = WebView::layout("Sign up", buildSignupBody(), nullptr,
                                       "signup", humanMessage(result), "error");
    return htmlResponse(html);
}

LogoutHandler::LogoutHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* LogoutHandler::callback(Request* req) {
    User* user = resolveUser(req);
    std::string sessionId = req->getSessionId();
    if (!sessionId.empty()) {
        sessions.removeSession(sessionId);
    }
    if (user != nullptr) {
        app.auth().setCurrentUser(user);
        app.auth().logout();
    }
    Response* res = redirectTo("/login");
    res->setSessionId("");
    return res;
}

HomeHandler::HomeHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* HomeHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user == nullptr) {
        return redirectTo("/login");
    }

    std::string html = WebView::layout("Welcome back",
                                       buildHomeBody(user),
                                       user, "home", "", "info");
    return htmlResponse(html);
}

AddRecipePageHandler::AddRecipePageHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* AddRecipePageHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user == nullptr) {
        return redirectTo("/login");
    }
    if (user->getRole() != CHEF) {
        return errorPage(humanMessage(Messages::PERMISSION_DENIED), user);
    }

    std::string html = WebView::layout("Add recipe",
                                       buildRecipeFormBody(),
                                       user, "chef/recipe", "", "info");
    return htmlResponse(html);
}

AddRecipeHandler::AddRecipeHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* AddRecipeHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user == nullptr) {
        return redirectTo("/login");
    }
    if (user->getRole() != CHEF) {
        return errorPage(humanMessage(Messages::PERMISSION_DENIED), user);
    }

    std::map<std::string, std::string> args;
    args[ArgKeys::TITLE] = req->getBodyParam("title");
    args[ArgKeys::INGREDIENTS] = req->getBodyParam("ingredients");
    args[ArgKeys::STEPS] = req->getBodyParam("steps");

    std::string result = app.commands().handlePostRecipe(args);
    std::string message;
    std::string type = "info";
    if (result != Messages::OK) {
        message = humanMessage(result);
        type = "error";
    } else {
        message = "Recipe saved successfully.";
    }

    std::string html = WebView::layout("Add recipe",
                                       buildRecipeFormBody(),
                                       user, "chef/recipe", message, type);
    return htmlResponse(html);
}

AddIngredientPageHandler::AddIngredientPageHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* AddIngredientPageHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user == nullptr) {
        return redirectTo("/login");
    }
    if (user->getRole() != VISITOR) {
        return errorPage(humanMessage(Messages::PERMISSION_DENIED), user);
    }

    std::string html = WebView::layout("Update fridge",
                                       buildIngredientFormBody(),
                                       user, "visitor/fridge", "", "info");
    return htmlResponse(html);
}

AddIngredientHandler::AddIngredientHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* AddIngredientHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user == nullptr) {
        return redirectTo("/login");
    }
    if (user->getRole() != VISITOR) {
        return errorPage(humanMessage(Messages::PERMISSION_DENIED), user);
    }

    std::map<std::string, std::string> args;
    args[ArgKeys::NAME] = req->getBodyParam("name");
    args[ArgKeys::AMOUNT] = req->getBodyParam("amount");

    std::string result = app.commands().handlePutIngredient(args);
    std::string message;
    std::string type = "info";
    if (result != Messages::OK) {
        message = humanMessage(result);
        type = "error";
    } else {
        message = "Fridge updated.";
    }

    std::string html = WebView::layout("Update fridge",
                                       buildIngredientFormBody(),
                                       user, "visitor/fridge", message, type);
    return htmlResponse(html);
}

RecommendationsPageHandler::RecommendationsPageHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* RecommendationsPageHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user == nullptr) {
        return redirectTo("/login");
    }
    if (user->getRole() != VISITOR) {
        return errorPage(humanMessage(Messages::PERMISSION_DENIED), user);
    }

    auto recommenders = app.recommender().getRecommenderNames();
    std::string html = WebView::layout("Recommendations",
                                       buildRecommendationsBody(recommenders, "", ""),
                                       user, "visitor/recommendations", "", "info");
    return htmlResponse(html);
}

RecommendationsHandler::RecommendationsHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* RecommendationsHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user == nullptr) {
        return redirectTo("/login");
    }
    if (user->getRole() != VISITOR) {
        return errorPage(humanMessage(Messages::PERMISSION_DENIED), user);
    }

    std::string selected = req->getBodyParam("name");
    auto recommenders = app.recommender().getRecommenderNames();
    std::string result = app.recommender().getRecommendations(selected, user);

    std::string message;
    std::string type = "info";
    std::string resultHtml;
    if (result == Messages::EMPTY || result == Messages::NOT_FOUND || result == Messages::BAD_REQUEST) {
        message = humanMessage(result);
        type = "error";
    } else {
        std::string safe = WebView::escape(result);
        resultHtml = "<pre class=\"output\">" + safe + "</pre>";
    }

    std::string html = WebView::layout("Recommendations",
                                       buildRecommendationsBody(recommenders, selected, resultHtml),
                                       user, "visitor/recommendations", message, type);
    return htmlResponse(html);
}

LikePageHandler::LikePageHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* LikePageHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user == nullptr) {
        return redirectTo("/login");
    }
    if (user->getRole() != VISITOR) {
        return errorPage(humanMessage(Messages::PERMISSION_DENIED), user);
    }

    std::string html = WebView::layout("Like a recipe",
                                       buildLikeBody(app.recipes()),
                                       user, "visitor/like", "", "info");
    return htmlResponse(html);
}

LikeHandler::LikeHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* LikeHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user == nullptr) {
        return redirectTo("/login");
    }
    if (user->getRole() != VISITOR) {
        return errorPage(humanMessage(Messages::PERMISSION_DENIED), user);
    }

    std::map<std::string, std::string> args;
    args[ArgKeys::RECIPE] = req->getBodyParam("recipe");
    std::string result = app.commands().handlePostLike(args);

    std::string message;
    std::string type = "info";
    if (result != Messages::OK) {
        message = humanMessage(result);
        type = "error";
    } else {
        message = "Thanks for supporting this recipe.";
    }

    std::string html = WebView::layout("Like a recipe",
                                       buildLikeBody(app.recipes()),
                                       user, "visitor/like", message, type);
    return htmlResponse(html);
}

MostLikedPageHandler::MostLikedPageHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* MostLikedPageHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user == nullptr) {
        return redirectTo("/login");
    }
    if (user->getRole() != CHEF) {
        return errorPage(humanMessage(Messages::PERMISSION_DENIED), user);
    }

    std::string html = WebView::layout("Most liked recipes",
                                       buildMostLikedBody("", ""),
                                       user, "chef/most-liked", "", "info");
    return htmlResponse(html);
}

MostLikedHandler::MostLikedHandler(WebApp& app, SessionManager& sessions)
    : BaseWebHandler(app, sessions) {}

Response* MostLikedHandler::callback(Request* req) {
    User* user = resolveUser(req);
    if (user == nullptr) {
        return redirectTo("/login");
    }
    if (user->getRole() != CHEF) {
        return errorPage(humanMessage(Messages::PERMISSION_DENIED), user);
    }

    std::string countValue = req->getBodyParam("count");
    std::map<std::string, std::string> args;
    args[ArgKeys::TYPE] = ReportTypes::MOST_LIKED;
    if (!countValue.empty()) {
        args[ArgKeys::COUNT] = countValue;
    }

    std::string result = app.commands().handleGetReport(args);
    std::string message;
    std::string type = "info";
    std::string resultHtml;
    if (result == Messages::EMPTY || result == Messages::BAD_REQUEST ||
        result == Messages::CHEF_HAS_NO_RECIPE || result == Messages::PERMISSION_DENIED) {
        message = humanMessage(result);
        type = "error";
    } else {
        std::string safe = WebView::escape(result);
        resultHtml = "<pre class=\"output\">" + safe + "</pre>";
    }

    std::string html = WebView::layout("Most liked recipes",
                                       buildMostLikedBody(countValue, resultHtml),
                                       user, "chef/most-liked", message, type);
    return htmlResponse(html);
}
