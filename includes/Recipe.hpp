#ifndef RECIPE_HPP
#define RECIPE_HPP

#include "global.hpp"

class Recipe {
private:
    string name;
    vector<pair<string, int>> ingredients;
    vector<string> steps;

public:
    Recipe(const string& name);

    void addIngredient(const string& ingredientName, int amount);
    void addStep(const string& step);

    const string& getName() const;
    const vector<pair<string, int>>& getIngredients() const;
    const vector<string>& getSteps() const;
};

#endif
