#include "Recipe.hpp"

Recipe::Recipe(const string& name) : name(name) {}

void Recipe::addIngredient(const string& ingredientName, int amount) {
    ingredients.push_back({ingredientName, amount});
}

void Recipe::addStep(const string& step) {
    steps.push_back(step);
}

const string& Recipe::getName() const {
    return name;
}

const vector<pair<string, int>>& Recipe::getIngredients() const {
    return ingredients;
}

const vector<string>& Recipe::getSteps() const {
    return steps;
}
