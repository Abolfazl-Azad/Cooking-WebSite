#include "WebApp.hpp"

#include "IngredientLoader.hpp"
#include "RecipeLoader.hpp"

WebApp::WebApp()
    : commandHandler(nullptr),
      recommenderSystem(nullptr) {}

WebApp::~WebApp() {
    delete commandHandler;
    delete recommenderSystem;
}

void WebApp::loadData(const std::string& ingredientPath, const std::string& recipePath) {
    IngredientLoader ingLoader;
    allIngredients = ingLoader.loadFromFile(ingredientPath);

    RecipeLoader recipeLoader;
    allRecipes = recipeLoader.loadFromFile(recipePath);

    delete commandHandler;
    delete recommenderSystem;

    commandHandler = new CommandHandler(allRecipes, allIngredients, authManager);
    recommenderSystem = new RecommenderSystem(allRecipes, allIngredients);
}

AuthManager& WebApp::auth() {
    return authManager;
}

CommandHandler& WebApp::commands() {
    return *commandHandler;
}

RecommenderSystem& WebApp::recommender() {
    return *recommenderSystem;
}

const std::vector<Recipe>& WebApp::recipes() const {
    return allRecipes;
}
