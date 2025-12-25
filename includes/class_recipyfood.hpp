#ifndef CLASS_RECIPYFOOD_H
#define CLASS_RECIPYFOOD_H

#include "global.hpp"
#include "User.hpp"
#include "Recipe.hpp"
#include "Ingredient.hpp"
#include "AuthManager.hpp"
#include "CommandHandler.hpp"
#include "RecommenderSystem.hpp"
#include "InputParser.hpp"
#include <sstream>

class recipyfood {
private:
    AuthManager authManager;
    CommandHandler* commandHandler;
    RecommenderSystem* recommenderSystem;
    
    vector<Ingredient> allIngredients;
    vector<Recipe> allRecipes;

public:
    recipyfood();
    ~recipyfood();
    
    void read_file(string ingredientPath, string recipePath);
    string process(string line);
};

#endif