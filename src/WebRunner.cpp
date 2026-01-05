#include "WebRunner.hpp"

#include <iostream>
#include <string>

#include "SessionManager.hpp"
#include "WebApp.hpp"
#include "WebHandlers.hpp"

#include "server/server.hpp"

int WebRunner::run(int argc, char* argv[]) {
    if (argc < 4) {
        return 1;
    }

    int port = 0;
    try {
        port = std::stoi(argv[1]);
    } catch (...) {
        return 1;
    }

    std::string ingredientsPath = argv[2];
    std::string recipesPath = argv[3];

    WebApp app;
    app.loadData(ingredientsPath, recipesPath);
    SessionManager sessions;

    try {
        Server server(port);
        server.setNotFoundErrPage("static/404.html");

        server.get("/", new RootHandler(app, sessions));
        server.get("/login", new LoginPageHandler(app, sessions));
        server.post("/login", new LoginHandler(app, sessions));
        server.get("/signup", new SignupPageHandler(app, sessions));
        server.post("/signup", new SignupHandler(app, sessions));
        server.get("/logout", new LogoutHandler(app, sessions));
        server.get("/home", new HomeHandler(app, sessions));

        server.get("/chef/recipe", new AddRecipePageHandler(app, sessions));
        server.post("/chef/recipe", new AddRecipeHandler(app, sessions));
        server.get("/chef/most-liked", new MostLikedPageHandler(app, sessions));
        server.post("/chef/most-liked", new MostLikedHandler(app, sessions));

        server.get("/visitor/fridge", new AddIngredientPageHandler(app, sessions));
        server.post("/visitor/fridge", new AddIngredientHandler(app, sessions));
        server.get("/visitor/recommendations", new RecommendationsPageHandler(app, sessions));
        server.post("/visitor/recommendations", new RecommendationsHandler(app, sessions));
        server.get("/visitor/like", new LikePageHandler(app, sessions));
        server.post("/visitor/like", new LikeHandler(app, sessions));

        server.get("/static/style.css", new ShowFile("static/style.css", "text/css"));
        server.get("/static/app.js", new ShowFile("static/app.js", "application/javascript"));

        std::cout << "Server running on port: " << port << std::endl;
        server.run();
    } catch (const Server::Exception& e) {
        std::cerr << e.getMessage() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
