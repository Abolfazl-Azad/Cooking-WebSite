#ifndef RECIPE_LOADER_HPP
#define RECIPE_LOADER_HPP

#include "Recipe.hpp"

class RecipeLoader {
public:
    vector<Recipe> loadFromFile(const string& filePath);

private:
    vector<string> split(const string& str, char delimiter);
    void parseIngredients(const string& text, Recipe& recipe);
    void parseSteps(const string& text, Recipe& recipe);
};

#endif
