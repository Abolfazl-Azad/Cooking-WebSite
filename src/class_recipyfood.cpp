#include "class_recipyfood.hpp"
#include "IngredientLoader.hpp"
#include "RecipeLoader.hpp"

recipyfood::recipyfood()
    : commandHandler(nullptr), recommenderSystem(nullptr) {}

recipyfood::~recipyfood()
{
    delete commandHandler;
    delete recommenderSystem;
}

void recipyfood::read_file(string ingredientPath, string recipePath)
{
    IngredientLoader ingLoader;
    this->allIngredients = ingLoader.loadFromFile(ingredientPath);

    RecipeLoader recLoader;
    this->allRecipes = recLoader.loadFromFile(recipePath);

    commandHandler = new CommandHandler(allRecipes, allIngredients, authManager);
    recommenderSystem = new RecommenderSystem(allRecipes, allIngredients);
}

string recipyfood::process(string line)
{
    stringstream ss(line);
    string method, command;

    ss >> method >> command;

    if (method != HttpMethods::POST && method != HttpMethods::GET&& method != HttpMethods::PUT && method != HttpMethods::DELETE)
    {
        return Messages::BAD_REQUEST;
    }

    size_t qPos = command.find('?');
    if (qPos != string::npos)
    {
        command = command.substr(0, qPos);
    }
    else
    {
        char next = ss.peek();
        while (next == ' ')
        {
            ss.get();
            next = ss.peek();
        }
        if (next == '?')
            ss.get();
    }

    map<string, string> args;
    if (method == HttpMethods::POST && command == Commands::SUGGESTION) {
        string action;
        if (!(ss >> action)) {
            return Messages::BAD_REQUEST;
        }
        args = InputParser::parseArguments(ss);
        args[ArgKeys::ACTION] = action;
    } else if (method == HttpMethods::GET && command == Commands::REPORT) {
        string firstToken;
        if (!(ss >> firstToken)) {
            return Messages::BAD_REQUEST;
        }
        if (firstToken == ArgKeys::TYPE) {
            string typeValue;
            if (!(ss >> typeValue)) {
                return Messages::BAD_REQUEST;
            }
            args = InputParser::parseArguments(ss);
            args[ArgKeys::TYPE] = typeValue;
        } else {
            args = InputParser::parseArguments(ss);
            args[ArgKeys::TYPE] = firstToken;
        }
    } else {
        args = InputParser::parseArguments(ss);
    }

    if (method == HttpMethods::POST)
    {
        if (command == Commands::SIGNUP)
        {
            if (args.find(ArgKeys::ROLE) == args.end() ||
                args.find(ArgKeys::USERNAME) == args.end() ||
                args.find(ArgKeys::PASSWORD) == args.end())
            {
                return Messages::BAD_REQUEST;
            }
            return authManager.signup(args[ArgKeys::ROLE], args[ArgKeys::USERNAME], args[ArgKeys::PASSWORD]);
        }

        if (command == Commands::LOGIN)
        {
            if (args.find(ArgKeys::USERNAME) == args.end() ||
                args.find(ArgKeys::PASSWORD) == args.end())
            {
                return Messages::BAD_REQUEST;
            }
            return authManager.login(args[ArgKeys::USERNAME], args[ArgKeys::PASSWORD]);
        }

        if (command == Commands::LOGOUT)
        {
            return authManager.logout();
        }

        if (command == Commands::RECIPE)
        {
            return commandHandler->handlePostRecipe(args);
        }

        if (command == Commands::LIKE)
        {
            return commandHandler->handlePostLike(args);
        }

        if (command == Commands::SUGGESTION)
        {
            return commandHandler->handlePostSuggestion(args);
        }

        if (command == Commands::ADD_RECOMMENDER)
        {
            if (!authManager.isLoggedIn())
            {
                return Messages::PERMISSION_DENIED;
            }
            if (authManager.getCurrentUser()->getRole() != VISITOR)
            {
                return Messages::PERMISSION_DENIED;
            }
            if (args.find(ArgKeys::NAME) == args.end() ||
                args.find(ArgKeys::INGREDIENTS) == args.end())
            {
                return Messages::BAD_REQUEST;
            }

            vector<string> ingredients = InputParser::splitString(args[ArgKeys::INGREDIENTS],
                                                                  DELIMITER_INGREDIENTS);
            return recommenderSystem->addRecommender(args[ArgKeys::NAME], ingredients);
        }
    }
    else if (method == HttpMethods::PUT)
    {
        if (command == Commands::INGREDIENT)
        {
            return commandHandler->handlePutIngredient(args);
        }
    }
    else if (method == HttpMethods::GET)
    {
        if (command == Commands::RECOMMENDERS)
        {
            if (!authManager.isLoggedIn())
            {
                return Messages::PERMISSION_DENIED;
            }
            if (authManager.getCurrentUser()->getRole() != VISITOR)
            {
                return Messages::PERMISSION_DENIED;
            }
            return recommenderSystem->getRecommendersList();
        }

        if (command == Commands::RECOMMENDATIONS)
        {
            if (!authManager.isLoggedIn())
            {
                return Messages::PERMISSION_DENIED;
            }
            if (authManager.getCurrentUser()->getRole() != VISITOR)
            {
                return Messages::PERMISSION_DENIED;
            }
            if (args.find(ArgKeys::NAME) == args.end())
            {
                return Messages::BAD_REQUEST;
            }
            return recommenderSystem->getRecommendations(args[ArgKeys::NAME],
                                                         authManager.getCurrentUser());
        }

        if (command == Commands::REPORT)
        {
            return commandHandler->handleGetReport(args);
        }
    }

    return Messages::NOT_FOUND;
}
