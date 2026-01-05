#include "CommandHandler.hpp"
#include "InputParser.hpp"

CommandHandler::CommandHandler(vector<Recipe>& recipes, 
                               vector<Ingredient>& ingredients,
                               AuthManager& auth)
    : allRecipes(recipes),
      allIngredients(ingredients),
      authManager(auth),
      suggestionHandler(recipes, ingredients),
      reportHandler(recipes) {}

string CommandHandler::toLowerCase(string str) {
    return InputParser::toLowerCase(str);
}

bool CommandHandler::recipeExists(string title) {
    return findRecipe(title) != nullptr;
}

Recipe* CommandHandler::findRecipe(const string& title) {
    for (auto& r : allRecipes) {
        if (r.getName() == title)
            return &r;
    }
    return nullptr;
}

bool CommandHandler::isValidIngredient(string name) {
    string searchName = toLowerCase(name);
    for (const auto& ing : allIngredients) {
        if (toLowerCase(ing.getName()) == searchName)
            return true;
    }
    return false;
}

string CommandHandler::handlePostRecipe(map<string, string>& args) {
    User* currentUser = authManager.getCurrentUser();
    
    if (currentUser == nullptr)
        return Messages::PERMISSION_DENIED;
    
    if (currentUser->getRole() != CHEF)
        return Messages::PERMISSION_DENIED;
    
    if (args.find(ArgKeys::TITLE) == args.end() ||
        args.find(ArgKeys::INGREDIENTS) == args.end() ||
        args.find(ArgKeys::STEPS) == args.end()) {
        return Messages::BAD_REQUEST;
    }
    
    string title = args[ArgKeys::TITLE];
    string ingredientsStr = args[ArgKeys::INGREDIENTS];
    string stepsStr = args[ArgKeys::STEPS];
    
    if (recipeExists(title)) {
        return Messages::ALREADY_EXISTS;
    }
    
    Recipe newRecipe(title);
    newRecipe.setOwner(currentUser->getUsername());
    
    vector<string> ingParts = InputParser::splitString(ingredientsStr, ';');
    for (const string& part : ingParts) {
        size_t colonPos = part.find(DELIMITER_KEY_VALUE);
        if (colonPos == string::npos)
            return Messages::BAD_REQUEST;
        
        string ingName = part.substr(0, colonPos);
        int amount;
        try {
            amount = stoi(part.substr(colonPos + 1));
        } catch (...) {
            return Messages::BAD_REQUEST;
        }
        
        if (amount <= 0)
            return Messages::BAD_REQUEST;
        
        if (!isValidIngredient(ingName))
            return Messages::NOT_FOUND;
        
        newRecipe.addIngredient(ingName, amount);
    }
    
    vector<string> stepParts = InputParser::splitString(stepsStr, ';');
    for (const string& step : stepParts) {
        newRecipe.addStep(step);
    }
    
    allRecipes.push_back(newRecipe);
    return Messages::OK;
}

string CommandHandler::handlePostLike(map<string, string>& args) {
    User* currentUser = authManager.getCurrentUser();
    if (currentUser == nullptr || currentUser->getRole() != VISITOR) {
        return Messages::PERMISSION_DENIED;
    }

    if (args.find(ArgKeys::RECIPE) == args.end()) {
        return Messages::BAD_REQUEST;
    }

    Recipe* recipe = findRecipe(args[ArgKeys::RECIPE]);
    if (recipe == nullptr) {
        return Messages::NOT_FOUND;
    }

    recipe->addLike(currentUser->getUsername());
    return Messages::OK;
}

string CommandHandler::handlePostSuggestion(map<string, string>& args) {
    return suggestionHandler.handlePostSuggestion(authManager.getCurrentUser(), args);
}

string CommandHandler::handlePutIngredient(map<string, string>& args) {
    User* currentUser = authManager.getCurrentUser();
    
    if (currentUser == nullptr)
        return Messages::PERMISSION_DENIED;
    
    if (currentUser->getRole() != VISITOR)
        return Messages::PERMISSION_DENIED;
    
    if (args.find(ArgKeys::NAME) == args.end() || args.find(ArgKeys::AMOUNT) == args.end()) {
        return Messages::BAD_REQUEST;
    }
    
    string name = args[ArgKeys::NAME];
    int amount;
    
    try {
        amount = stoi(args[ArgKeys::AMOUNT]);
    } catch (...) {
        return Messages::BAD_REQUEST;
    }
    
    if (amount <= 0)
        return Messages::BAD_REQUEST;
    
    if (!isValidIngredient(name)) {
        return Messages::NOT_FOUND;
    }
    
    currentUser->addToShelf(name, amount);
    return Messages::OK;
}

string CommandHandler::handleGetReport(map<string, string>& args) {
    User* currentUser = authManager.getCurrentUser();
    
    if (currentUser == nullptr) {
        return Messages::PERMISSION_DENIED;
    }
    
    return reportHandler.handleReport(currentUser, args);
}
