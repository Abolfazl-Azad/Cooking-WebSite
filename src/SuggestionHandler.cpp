#include "SuggestionHandler.hpp"
#include "InputParser.hpp"

SuggestionHandler::SuggestionHandler(vector<Recipe>& recipes, vector<Ingredient>& ingredients)
    : allRecipes(recipes), allIngredients(ingredients) {}

Recipe* SuggestionHandler::findRecipe(const string& title) {
    for (auto& recipe : allRecipes) {
        if (recipe.getName() == title) {
            return &recipe;
        }
    }
    return nullptr;
}

bool SuggestionHandler::isValidIngredient(string name) {
    string searchName = InputParser::toLowerCase(name);
    for (const auto& ing : allIngredients) {
        if (InputParser::toLowerCase(ing.getName()) == searchName) {
            return true;
        }
    }
    return false;
}

bool SuggestionHandler::recipeHasIngredient(const Recipe& recipe, const string& ingredientName) {
    string searchName = InputParser::toLowerCase(ingredientName);
    for (const auto& ing : recipe.getIngredients()) {
        if (InputParser::toLowerCase(ing.first) == searchName) {
            return true;
        }
    }
    return false;
}

string SuggestionHandler::handlePostSuggestion(User* currentUser, map<string, string>& args) {
    if (currentUser == nullptr || currentUser->getRole() != VISITOR) {
        return Messages::PERMISSION_DENIED;
    }

    if (args.find(ArgKeys::ACTION) == args.end()) {
        return Messages::BAD_REQUEST;
    }

    string action = args[ArgKeys::ACTION];
    if (action == "add_ingredient") {
        return handleAddIngredientSuggestion(currentUser, args);
    }
    if (action == "modify_ingredient") {
        return handleModifyIngredientSuggestion(currentUser, args);
    }
    return Messages::BAD_REQUEST;
}

string SuggestionHandler::handleAddIngredientSuggestion(User* currentUser, map<string, string>& args) {
    (void)currentUser;
    if (args.find(ArgKeys::RECIPE) == args.end() ||
        args.find(ArgKeys::INGREDIENT) == args.end() ||
        args.find(ArgKeys::AMOUNT) == args.end()) {
        return Messages::BAD_REQUEST;
    }

    int amount;
    try {
        amount = stoi(args[ArgKeys::AMOUNT]);
    } catch (...) {
        return Messages::BAD_REQUEST;
    }

    if (amount <= 0) {
        return Messages::BAD_REQUEST;
    }

    Recipe* recipe = findRecipe(args[ArgKeys::RECIPE]);
    if (recipe == nullptr) {
        return Messages::NOT_FOUND;
    }

    if (!isValidIngredient(args[ArgKeys::INGREDIENT])) {
        return Messages::NOT_FOUND;
    }

    if (recipeHasIngredient(*recipe, args[ArgKeys::INGREDIENT])) {
        return Messages::BAD_REQUEST;
    }

    return Messages::OK;
}

string SuggestionHandler::handleModifyIngredientSuggestion(User* currentUser, map<string, string>& args) {
    (void)currentUser;
    if (args.find(ArgKeys::RECIPE) == args.end() ||
        args.find(ArgKeys::INGREDIENT) == args.end() ||
        args.find(ArgKeys::AMOUNT) == args.end()) {
        return Messages::BAD_REQUEST;
    }

    int amount;
    try {
        amount = stoi(args[ArgKeys::AMOUNT]);
    } catch (...) {
        return Messages::BAD_REQUEST;
    }

    if (amount <= 0) {
        return Messages::BAD_REQUEST;
    }

    Recipe* recipe = findRecipe(args[ArgKeys::RECIPE]);
    if (recipe == nullptr) {
        return Messages::NOT_FOUND;
    }

    if (!isValidIngredient(args[ArgKeys::INGREDIENT])) {
        return Messages::NOT_FOUND;
    }

    if (!recipeHasIngredient(*recipe, args[ArgKeys::INGREDIENT])) {
        return Messages::BAD_REQUEST;
    }

    return Messages::OK;
}
