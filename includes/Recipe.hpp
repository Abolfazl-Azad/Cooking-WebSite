#ifndef RECIPE_HPP
#define RECIPE_HPP

#include "global.hpp"
#include <unordered_set>

class Recipe {
private:
    string name;
    vector<pair<string, int>> ingredients;
    vector<string> steps;
    string owner;
    int likes;
    unordered_set<string> likedBy;

public:
    Recipe(const string& name);

    void addIngredient(const string& ingredientName, int amount);
    void addStep(const string& step);
    void setOwner(const string& ownerName);
    const string& getOwner() const;
    int getLikes() const;
    bool addLike(const string& username);

    const string& getName() const;
    const vector<pair<string, int>>& getIngredients() const;
    const vector<string>& getSteps() const;
};

#endif
