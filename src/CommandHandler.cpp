#include "CommandHandler.hpp"
#include "InputParser.hpp"

CommandHandler::CommandHandler(vector<Recipe>& recipes, 
                               vector<Ingredient>& ingredients,
                               AuthManager& auth)
    : allRecipes(recipes), allIngredients(ingredients), authManager(auth) {}

string CommandHandler::toLowerCase(string str) {
    return InputParser::toLowerCase(str);
}

bool CommandHandler::recipeExists(string title) {
    for (const auto& r : allRecipes) {
        if (r.getName() == title)
            return true;
    }
    return false;
}

bool CommandHandler::isValidIngredient(string name) {
    string searchName = toLowerCase(name);
    for (const auto& ing : allIngredients) {
        if (toLowerCase(ing.getName()) == searchName)
            return true;
    }
    return false;
}

string CommandHandler::getIngredientCategory(string name) {
    string searchName = toLowerCase(name);
    for (const auto& ing : allIngredients) {
        if (toLowerCase(ing.getName()) == searchName)
            return ing.getCategory();
    }
    return "";
}

int CommandHandler::getIngredientPrice(string name) {
    string searchName = toLowerCase(name);
    for (const auto& ing : allIngredients) {
        if (toLowerCase(ing.getName()) == searchName)
            return ing.getPrice();
    }
    return 0;
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
    
    if (args.find(ArgKeys::TYPE) == args.end() || args[ArgKeys::TYPE] != "refrigerator_status") {
        return Messages::BAD_REQUEST;
    }
    
    auto shelfMap = currentUser->getShelf();
    
    if (shelfMap.empty()) {
        return Messages::NO_ITEMS_IN_FRIDGE;
    }
    
    vector<pair<string, int>> shelfVec;
    for (auto const& item : shelfMap) {
        shelfVec.push_back(item);
    }
    
    sort(shelfVec.begin(), shelfVec.end(), 
         [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.first > b.first;
    });
    
    string output = OutputFormats::FRIDGE_HEADER + currentUser->getUsername() + ":\n";
    
    for (size_t i = 0; i < shelfVec.size(); ++i) {
        output += shelfVec[i].first + ":" + to_string(shelfVec[i].second);
        if (i != shelfVec.size() - 1) {
            output += "\n";
        }
    }
    
    return output;
}