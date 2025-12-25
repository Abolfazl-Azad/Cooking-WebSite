#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include "global.hpp"
#include "Recipe.hpp"
#include "Ingredient.hpp"
#include "User.hpp"
#include "AuthManager.hpp"

class CommandHandler {
private:
    vector<Recipe>& allRecipes;
    vector<Ingredient>& allIngredients;
    AuthManager& authManager;
    
    bool recipeExists(string title);
    bool isValidIngredient(string name);
    string getIngredientCategory(string name);
    int getIngredientPrice(string name);
    string toLowerCase(string str);

public:
    CommandHandler(vector<Recipe>& recipes, 
                   vector<Ingredient>& ingredients,
                   AuthManager& auth);
    
    string handlePostRecipe(map<string, string>& args);
    string handlePutIngredient(map<string, string>& args);
    string handleGetReport(map<string, string>& args);
};

#endif