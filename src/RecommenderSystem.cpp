#include "RecommenderSystem.hpp"
#include "InputParser.hpp"

RecommenderSystem::RecommenderSystem(vector<Recipe>& recipes, vector<Ingredient>& ingredients)
    : allRecipes(recipes), allIngredients(ingredients) {}

string RecommenderSystem::toLowerCase(string str) {
    return InputParser::toLowerCase(str);
}

string RecommenderSystem::getRecommendersList() {
    vector<string> names = {
        Recommenders::MATCH_BASED,
        Recommenders::COST_BASED,
        Recommenders::CALORIE_BASED,
        Recommenders::MOST_LIKED_BASE
    };

    for (const auto& custom : customRecommenders) {
        names.push_back(custom.name);
    }

    string output = "recommenders:\n";
    for (size_t i = 0; i < names.size(); ++i) {
        output += to_string(i + 1) + ". " + names[i];
        if (i != names.size() - 1) {
            output += "\n";
        }
    }
    return output;
}

string RecommenderSystem::getRecommendations(string recommenderName, User* user) {
    vector<Recipe> finalRecipes;
    bool foundCustom = false;
    
    if (recommenderName == Recommenders::MATCH_BASED) {
        finalRecipes = matchBasedAlgorithm(user);
    } else if (recommenderName == Recommenders::COST_BASED) {
        finalRecipes = costBasedAlgorithm(user);
    } else if (recommenderName == Recommenders::CALORIE_BASED) {
        finalRecipes = calorieBasedAlgorithm(user);
    } else if (recommenderName == Recommenders::MOST_LIKED_BASE) {
        finalRecipes = mostLikedAlgorithm();
    } else {
        for (const auto& custom : customRecommenders) {
            if (custom.name == recommenderName) {
                finalRecipes = customRecommenderAlgorithm(custom.ingredients);
                foundCustom = true;
                break;
            }
        }
        if (!foundCustom) {
            return Messages::NOT_FOUND;
        }
    }
    
    if (finalRecipes.empty())
        return Messages::EMPTY;
    
    string output = recommenderName + "\n";
    size_t limit = 3;
    
    for (size_t i = 0; i < finalRecipes.size() && i < limit; ++i) {
        output += formatRecipeOutput(finalRecipes[i]);
        if (i != finalRecipes.size() - 1 && i != limit - 1)
            output += "\n";
    }
    
    return output;
}

vector<Recipe> RecommenderSystem::matchBasedAlgorithm(const User* user) {
    vector<pair<Recipe, int>> scored;
    auto userShelf = user->getShelf();
    unordered_set<string> available;
    available.reserve(userShelf.size());

    for (const auto& item : userShelf) {
        available.insert(toLowerCase(item.first));
    }

    for (const auto& r : allRecipes) {
        int overlapCount = 0;
        int totalCount = 0;
        unordered_set<string> seen;

        for (const auto& ing : r.getIngredients()) {
            string ingName = toLowerCase(ing.first);
            if (seen.insert(ingName).second) {
                ++totalCount;
                if (available.find(ingName) != available.end()) {
                    ++overlapCount;
                }
            }
        }

        scored.push_back({r, totalCount - overlapCount});
    }

    sort(scored.begin(), scored.end(),
         [](const pair<Recipe, int>& a, const pair<Recipe, int>& b) {
        if (a.second != b.second) {
            return a.second < b.second;
        }
        return a.first.getName() > b.first.getName();
    });

    vector<Recipe> matches;
    matches.reserve(scored.size());
    for (const auto& item : scored) {
        matches.push_back(item.first);
    }

    return matches;
}

vector<Recipe> RecommenderSystem::costBasedAlgorithm(const User* user) {
    vector<Recipe> allRecipesWithCost = allRecipes;
    auto userShelf = user->getShelf();
    
    sort(allRecipesWithCost.begin(), allRecipesWithCost.end(), 
         [this, &userShelf](const Recipe& a, const Recipe& b) {
        int costA = this->calculateAdditionalCost(a, userShelf);
        int costB = this->calculateAdditionalCost(b, userShelf);
        
        if (costA != costB) {
            return costA < costB;
        }
        return a.getName() > b.getName();
    });
    
    return allRecipesWithCost;
}

vector<Recipe> RecommenderSystem::calorieBasedAlgorithm(const User* user) {
    vector<Recipe> sortedRecipes = allRecipes;
    
    sort(sortedRecipes.begin(), sortedRecipes.end(), 
         [this](const Recipe& a, const Recipe& b) {
        int calA = this->calculateRecipeCalories(a);
        int calB = this->calculateRecipeCalories(b);
        
        if (calA != calB) {
            return calA < calB;
        }
        return a.getName() < b.getName();
    });
    
    return sortedRecipes;
}

vector<Recipe> RecommenderSystem::mostLikedAlgorithm() {
    vector<Recipe> sortedRecipes = allRecipes;
    sort(sortedRecipes.begin(), sortedRecipes.end(),
         [](const Recipe& a, const Recipe& b) {
        if (a.getLikes() != b.getLikes()) {
            return a.getLikes() > b.getLikes();
        }
        return a.getName() > b.getName();
    });
    return sortedRecipes;
}

vector<Recipe> RecommenderSystem::customRecommenderAlgorithm(const vector<string>& ingredients) {
    vector<Recipe> matches;
    for (const auto& recipe : allRecipes) {
        bool hasAll = true;
        for (const auto& ing : ingredients) {
            if (!recipeHasIngredient(recipe, ing)) {
                hasAll = false;
                break;
            }
        }
        if (hasAll) {
            matches.push_back(recipe);
        }
    }

    sort(matches.begin(), matches.end(),
         [](const Recipe& a, const Recipe& b) {
        if (a.getLikes() != b.getLikes()) {
            return a.getLikes() > b.getLikes();
        }
        return a.getName() > b.getName();
    });

    return matches;
}

int RecommenderSystem::calculateRecipeCalories(const Recipe& recipe) {
    int totalCal = 0;
    auto recipeIngs = recipe.getIngredients();
    
    for (auto& pair : recipeIngs) {
        string category = getIngredientCategory(pair.first);
        
        if (!category.empty() && CATEGORY_CALORIES.find(category) != CATEGORY_CALORIES.end()) {
            totalCal += pair.second * CATEGORY_CALORIES.at(category);
        }
    }
    return totalCal;
}

int RecommenderSystem::calculateRecipeCost(const Recipe& recipe) {
    int totalCost = 0;
    auto recipeIngs = recipe.getIngredients();
    
    for (auto& pair : recipeIngs) {
        totalCost += pair.second * getIngredientPrice(pair.first);
    }
    return totalCost;
}

int RecommenderSystem::calculateAdditionalCost(const Recipe& recipe, 
                                                const map<string, int>& userShelf) {
    int additionalCost = 0;
    auto recipeIngs = recipe.getIngredients();
    
    for (auto& pair : recipeIngs) {
        string ingName = toLowerCase(pair.first);
        int required = pair.second;
        
        int available = 0;
        if (userShelf.find(ingName) != userShelf.end()) {
            available = userShelf.at(ingName);
        }
        
        int needed = max(0, required - available);
        if (needed > 0) {
            additionalCost += needed * getIngredientPrice(pair.first);
        }
    }
    
    return additionalCost;
}

string RecommenderSystem::formatRecipeOutput(const Recipe& recipe) {
    string res = recipe.getName() + ": \n";
    
    res += "Ingredients: ";
    auto ings = recipe.getIngredients();
    for (size_t i = 0; i < ings.size(); ++i) {
        res += ings[i].first + ":" + to_string(ings[i].second);
        if (i != ings.size() - 1)
            res += ", ";
    }
    res += " \n";
    
    res += OutputFormats::CALORIES_LABEL + to_string(calculateRecipeCalories(recipe)) + "\n";
    
    res += OutputFormats::STEPS_LABEL;
    auto steps = recipe.getSteps();
    for (const string& s : steps) {
        res += s + "\n";
    }
    
    if (!res.empty() && res.back() == '\n')
        res.pop_back();
    
    return res;
}

string RecommenderSystem::getIngredientCategory(string name) {
    string searchName = toLowerCase(name);
    
    for (const auto& ing : allIngredients) {
        if (toLowerCase(ing.getName()) == searchName)
            return ing.getCategory();
    }
    return "";
}

int RecommenderSystem::getIngredientPrice(string name) {
    string searchName = toLowerCase(name);
    
    for (const auto& ing : allIngredients) {
        if (toLowerCase(ing.getName()) == searchName)
            return ing.getPrice();
    }
    return 0;
}

bool RecommenderSystem::isValidIngredient(string name) {
    string searchName = toLowerCase(name);
    for (const auto& ing : allIngredients) {
        if (toLowerCase(ing.getName()) == searchName) {
            return true;
        }
    }
    return false;
}

bool RecommenderSystem::recipeHasIngredient(const Recipe& recipe, const string& ingredientName) {
    string searchName = toLowerCase(ingredientName);
    for (const auto& ing : recipe.getIngredients()) {
        if (toLowerCase(ing.first) == searchName) {
            return true;
        }
    }
    return false;
}

bool RecommenderSystem::recommenderNameExists(const string& name) {
    if (name == Recommenders::MATCH_BASED ||
        name == Recommenders::COST_BASED ||
        name == Recommenders::CALORIE_BASED ||
        name == Recommenders::MOST_LIKED_BASE) {
        return true;
    }
    for (const auto& custom : customRecommenders) {
        if (custom.name == name) {
            return true;
        }
    }
    return false;
}

string RecommenderSystem::addRecommender(const string& name, const vector<string>& ingredients) {
    if (name.empty() || ingredients.empty()) {
        return Messages::BAD_REQUEST;
    }

    if (recommenderNameExists(name)) {
        return Messages::ALREADY_EXISTS;
    }

    vector<string> normalizedIngredients;
    for (const auto& ing : ingredients) {
        if (ing.empty()) {
            return Messages::BAD_REQUEST;
        }
        if (!isValidIngredient(ing)) {
            return Messages::NOT_FOUND;
        }
        normalizedIngredients.push_back(toLowerCase(ing));
    }

    sort(normalizedIngredients.begin(), normalizedIngredients.end());
    normalizedIngredients.erase(unique(normalizedIngredients.begin(), normalizedIngredients.end()),
                                normalizedIngredients.end());

    CustomRecommender custom{ name, normalizedIngredients };
    customRecommenders.push_back(custom);
    return Messages::OK;
}
