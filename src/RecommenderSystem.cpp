#include "RecommenderSystem.hpp"
#include "InputParser.hpp"

RecommenderSystem::RecommenderSystem(vector<Recipe>& recipes, vector<Ingredient>& ingredients)
    : allRecipes(recipes), allIngredients(ingredients) {}

string RecommenderSystem::toLowerCase(string str) {
    return InputParser::toLowerCase(str);
}

string RecommenderSystem::getRecommendersList() {

    return OutputFormats::RECOMMENDERS_LIST;
}

string RecommenderSystem::getRecommendations(string recommenderName, User* user) {
    vector<Recipe> finalRecipes;
    
    if (recommenderName == Recommenders::MATCH_BASED) {
        finalRecipes = matchBasedAlgorithm(user);
    } else if (recommenderName == Recommenders::COST_BASED) {
        finalRecipes = costBasedAlgorithm(user);
    } else if (recommenderName == Recommenders::CALORIE_BASED) {
        finalRecipes = calorieBasedAlgorithm(user);
    } else {
        return Messages::NOT_FOUND;
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
    vector<Recipe> matches;
    auto userShelf = user->getShelf();
    
    for (const auto& r : allRecipes) {
        bool canCook = true;
        for (const auto& ing : r.getIngredients()) {
            string searchName = toLowerCase(ing.first);
            int reqAmount = ing.second;
            
            if (userShelf.find(searchName) == userShelf.end() || 
                userShelf.at(searchName) < reqAmount) {
                canCook = false;
                break;
            }
        }
        if (canCook) {
            matches.push_back(r);
        }
    }
    
    sort(matches.begin(), matches.end(), [](const Recipe& a, const Recipe& b) {
        return a.getName() > b.getName();
    });
    
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
    string res = recipe.getName() + ":\n";
    
    res += "Ingredients: ";
    auto ings = recipe.getIngredients();
    for (size_t i = 0; i < ings.size(); ++i) {
        res += ings[i].first + ":" + to_string(ings[i].second);
        if (i != ings.size() - 1)
            res += ", ";
    }
    res += "\n";
    
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