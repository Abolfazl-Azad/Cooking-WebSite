#include "Recipe.hpp"

Recipe::Recipe(const string& name) : name(name), owner(""), likes(0) {}

void Recipe::addIngredient(const string& ingredientName, int amount) {
    ingredients.push_back({ingredientName, amount});
}

void Recipe::addStep(const string& step) {
    steps.push_back(step);
}

void Recipe::setOwner(const string& ownerName) {
    owner = ownerName;
}

const string& Recipe::getOwner() const {
    return owner;
}

int Recipe::getLikes() const {
    return likes;
}

bool Recipe::addLike(const string& username) {
    auto result = likedBy.insert(username);
    if (result.second) {
        ++likes;
        return true;
    }
    return false;
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
