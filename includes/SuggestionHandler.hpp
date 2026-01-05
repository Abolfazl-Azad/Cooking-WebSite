#ifndef SUGGESTION_HANDLER_HPP
#define SUGGESTION_HANDLER_HPP

#include "global.hpp"
#include "Recipe.hpp"
#include "Ingredient.hpp"
#include "User.hpp"

class SuggestionHandler {
private:
    vector<Recipe>& allRecipes;
    vector<Ingredient>& allIngredients;

    Recipe* findRecipe(const string& title);
    bool isValidIngredient(string name);
    bool recipeHasIngredient(const Recipe& recipe, const string& ingredientName);
    string handleAddIngredientSuggestion(User* currentUser, map<string, string>& args);
    string handleModifyIngredientSuggestion(User* currentUser, map<string, string>& args);

public:
    SuggestionHandler(vector<Recipe>& recipes, vector<Ingredient>& ingredients);
    string handlePostSuggestion(User* currentUser, map<string, string>& args);
};

#endif
