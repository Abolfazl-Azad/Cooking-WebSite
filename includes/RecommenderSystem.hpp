#ifndef RECOMMENDER_SYSTEM_HPP
#define RECOMMENDER_SYSTEM_HPP

#include "global.hpp"
#include "Recipe.hpp"
#include "Ingredient.hpp"
#include "User.hpp"

struct CustomRecommender {
    string name;
    vector<string> ingredients;
};

class RecommenderSystem {
private:
    vector<Recipe>& allRecipes;
    vector<Ingredient>& allIngredients;
    vector<CustomRecommender> customRecommenders;
    vector<Recipe> matchBasedAlgorithm(const User* user);
    vector<Recipe> costBasedAlgorithm(const User* user);
    vector<Recipe> calorieBasedAlgorithm(const User* user);
    vector<Recipe> mostLikedAlgorithm();
    vector<Recipe> customRecommenderAlgorithm(const vector<string>& ingredients);

    int calculateRecipeCalories(const Recipe& recipe);
    int calculateRecipeCost(const Recipe& recipe);
    int calculateAdditionalCost(const Recipe& recipe, const map<string, int>& userShelf);
    string formatRecipeOutput(const Recipe& recipe);
    string getIngredientCategory(string name);
    int getIngredientPrice(string name);
    string toLowerCase(string str);
    bool isValidIngredient(string name);
    bool recipeHasIngredient(const Recipe& recipe, const string& ingredientName);
    bool recommenderNameExists(const string& name);

public:
    RecommenderSystem(vector<Recipe>& recipes, vector<Ingredient>& ingredients);
    
    vector<string> getRecommenderNames() const;
    string getRecommendersList();
    string getRecommendations(string recommenderName, User* user);
    string addRecommender(const string& name, const vector<string>& ingredients);
};

#endif
