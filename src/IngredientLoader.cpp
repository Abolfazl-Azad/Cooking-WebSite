#include "IngredientLoader.hpp"

vector<string> IngredientLoader::split(const string &str, char delimiter)
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

vector<Ingredient> IngredientLoader::loadFromFile(const string &filePath)
{
    vector<Ingredient> ingredients;
    ifstream file(filePath);


    string line;

    getline(file, line);

    while (getline(file, line))
    {
        auto columns = split(line, ',');

        if (columns.size() < 3)
            continue;

        string name = columns[0];
        string category = columns[1];
        int price = stoi(columns[2]);

        ingredients.emplace_back(name, category, price);
    }

    return ingredients;
}
