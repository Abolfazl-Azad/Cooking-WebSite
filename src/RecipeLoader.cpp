#include "RecipeLoader.hpp"

vector<Recipe> RecipeLoader::loadFromFile(const string &filePath)
{
    ifstream file(filePath);
    vector<Recipe> recipes;
    string line;

    getline(file, line);
    while (getline(file, line))
    {
        auto columns = split(line, split_namefood);

        if (columns.size() < 3)
        {
            continue;
        }

        Recipe recipe(columns[0]);
        parseIngredients(columns[1], recipe);
        parseSteps(columns[2], recipe);

        recipes.push_back(recipe);
    }
    return recipes;
}

vector<string> RecipeLoader::split(const string &str, char delimiter)
{
    vector<string> result;
    stringstream ss(str);
    string item;

    while (getline(ss, item, delimiter))
    {
        result.push_back(item);
    }

    return result;
}

void RecipeLoader::parseIngredients(const string &text, Recipe &recipe)
{
    auto items = split(text, split_items_steps_food);

    for (const auto &item : items)
    {
        auto parts = split(item, split_parts);
        recipe.addIngredient(parts[0], stoi(parts[1]));
    }
}

void RecipeLoader::parseSteps(const string &text, Recipe &recipe)
{
    auto steps = split(text, split_items_steps_food);

    for (const auto &step : steps)
    {
        recipe.addStep(step);
    }
}
