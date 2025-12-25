#ifndef INGREDIENT_LOADER_HPP
#define INGREDIENT_LOADER_HPP

#include "Ingredient.hpp"

class IngredientLoader {
public:
    vector<Ingredient> loadFromFile(const string& filePath);

private:
    vector<string> split(const string& str, char delimiter);
};

#endif
