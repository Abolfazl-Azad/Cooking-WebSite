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

    map<string, string> args = InputParser::parseArguments(ss);

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
            return recommenderSystem->getRecommendersList();
        }

        if (command == Commands::RECOMMENDATIONS)
        {
            if (!authManager.isLoggedIn())
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