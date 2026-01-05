#ifndef WEB_HANDLERS_HPP
#define WEB_HANDLERS_HPP

#include <map>
#include <string>

#include "SessionManager.hpp"
#include "WebApp.hpp"

#include "server/server.hpp"

class BaseWebHandler : public RequestHandler {
public:
    BaseWebHandler(WebApp& app, SessionManager& sessions);

protected:
    WebApp& app;
    SessionManager& sessions;

    User* resolveUser(Request* req);
    Response* htmlResponse(const std::string& html);
    Response* redirectTo(const std::string& url, const std::string& sessionId = "");
    Response* errorPage(const std::string& message, User* user);
    std::string humanMessage(const std::string& result) const;
};

class RootHandler : public BaseWebHandler {
public:
    RootHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class LoginPageHandler : public BaseWebHandler {
public:
    LoginPageHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class LoginHandler : public BaseWebHandler {
public:
    LoginHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class SignupPageHandler : public BaseWebHandler {
public:
    SignupPageHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class SignupHandler : public BaseWebHandler {
public:
    SignupHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class LogoutHandler : public BaseWebHandler {
public:
    LogoutHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class HomeHandler : public BaseWebHandler {
public:
    HomeHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class AddRecipePageHandler : public BaseWebHandler {
public:
    AddRecipePageHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class AddRecipeHandler : public BaseWebHandler {
public:
    AddRecipeHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class AddIngredientPageHandler : public BaseWebHandler {
public:
    AddIngredientPageHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class AddIngredientHandler : public BaseWebHandler {
public:
    AddIngredientHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class RecommendationsPageHandler : public BaseWebHandler {
public:
    RecommendationsPageHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class RecommendationsHandler : public BaseWebHandler {
public:
    RecommendationsHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class LikePageHandler : public BaseWebHandler {
public:
    LikePageHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class LikeHandler : public BaseWebHandler {
public:
    LikeHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class MostLikedPageHandler : public BaseWebHandler {
public:
    MostLikedPageHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

class MostLikedHandler : public BaseWebHandler {
public:
    MostLikedHandler(WebApp& app, SessionManager& sessions);
    Response* callback(Request* req) override;
};

#endif
