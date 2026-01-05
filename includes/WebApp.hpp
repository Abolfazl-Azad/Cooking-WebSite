#ifndef WEB_APP_HPP
#define WEB_APP_HPP

#include "AuthManager.hpp"
#include "CommandHandler.hpp"
#include "Ingredient.hpp"
#include "RecommenderSystem.hpp"
#include "Recipe.hpp"

class WebApp {
public:
    WebApp();
    ~WebApp();

    void loadData(const std::string& ingredientPath, const std::string& recipePath);

    AuthManager& auth();
    CommandHandler& commands();
    RecommenderSystem& recommender();
    const std::vector<Recipe>& recipes() const;

private:
    AuthManager authManager;
    CommandHandler* commandHandler;
    RecommenderSystem* recommenderSystem;

    std::vector<Ingredient> allIngredients;
    std::vector<Recipe> allRecipes;
};

#endif
