#ifndef RECOMMENDER_SYSTEM_HPP
#define RECOMMENDER_SYSTEM_HPP

#include "global.hpp"
#include "Recipe.hpp"
#include "Ingredient.hpp"
#include "User.hpp"

class RecommenderSystem {
private:
    vector<Recipe>& allRecipes;
    vector<Ingredient>& allIngredients;
    vector<Recipe> matchBasedAlgorithm(const User* user);
    vector<Recipe> costBasedAlgorithm(const User* user);
    vector<Recipe> calorieBasedAlgorithm(const User* user);

    int calculateRecipeCalories(const Recipe& recipe);
    int calculateRecipeCost(const Recipe& recipe);
    int calculateAdditionalCost(const Recipe& recipe, const map<string, int>& userShelf);
    string formatRecipeOutput(const Recipe& recipe);
    string getIngredientCategory(string name);
    int getIngredientPrice(string name);
    string toLowerCase(string str);

public:
    RecommenderSystem(vector<Recipe>& recipes, vector<Ingredient>& ingredients);
    
    string getRecommendersList();
    string getRecommendations(string recommenderName, User* user);
};

#endif