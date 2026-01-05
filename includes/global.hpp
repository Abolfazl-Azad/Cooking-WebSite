
#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <unordered_map>
#include <cctype>

using namespace std;

const char DELIMITER_CSV = ',';
const char DELIMITER_INGREDIENTS = ';';
const char DELIMITER_KEY_VALUE = ':';
const char QUOTE_CHAR = '"';

const char split_namefood = DELIMITER_CSV;
const char split_items_steps_food = DELIMITER_INGREDIENTS;
const char split_parts = DELIMITER_KEY_VALUE;

const unordered_map<string, int> CATEGORY_CALORIES = {
    {"Vegetables", 25},
    {"Fruits", 50},
    {"Protein", 250},
    {"Dairy", 150},
    {"Grains and Carbohydrates", 100},
    {"Spices", 5},
    {"Fats and Oils", 120}
};

namespace Constants {
    const int MAX_RECOMMENDATIONS = 3;
    const int MIN_INGREDIENT_AMOUNT = 1;
    const int DEFAULT_CALORIE_VALUE = 0;
    const int DEFAULT_PRICE_VALUE = 0;
}

namespace Messages {
    const string OK = "OK";
    const string BAD_REQUEST = "Bad Request";
    const string NOT_FOUND = "Not Found";
    const string PERMISSION_DENIED = "Permission Denied";
    const string ALREADY_EXISTS = "Already Exists";
    const string EMPTY = "Empty";
    const string NO_ITEMS_IN_FRIDGE = "No item exists in the refrigerator.";
    const string CHEF_HAS_NO_RECIPE = "This chef has no recipe.";
}

namespace Commands {
    const string SIGNUP = "signup";
    const string LOGIN = "login";
    const string LOGOUT = "logout";
    const string RECIPE = "recipe";
    const string INGREDIENT = "ingredient";
    const string LIKE = "like";
    const string SUGGESTION = "suggestion";
    const string ADD_RECOMMENDER = "add_recommender";
    const string RECOMMENDERS = "recommenders";
    const string RECOMMENDATIONS = "recommendations";
    const string REPORT = "report";
}

namespace HttpMethods {
    const string POST = "POST";
    const string GET = "GET";
    const string PUT = "PUT";
    const string DELETE = "DELETE";
}

namespace Roles {
    const string CHEF = "chef";
    const string VISITOR = "visitor";
}

namespace Recommenders {
    const string MATCH_BASED = "Match based recommender";
    const string COST_BASED = "Cost based recommender";
    const string CALORIE_BASED = "Calorie based recommender";
    const string MOST_LIKED_BASE = "Most-liked base recommender";
}

namespace ReportTypes {
    const string REFRIGERATOR_STATUS = "refrigerator_status";
    const string MOST_LIKED = "most_liked";
}

namespace OutputFormats {
    const string RECOMMENDERS_LIST = "recommenders:\n1. Match based recommender\n2. Cost based recommender\n3. Calorie based recommender";
    const string FRIDGE_HEADER = "Refrigerator inventory for user ";
    const string INGREDIENTS_LABEL = "Ingredients: ";
    const string CALORIES_LABEL = "Calories: ";
    const string STEPS_LABEL = "Steps:\n";
}

namespace ArgKeys {
    const string ROLE = "role";
    const string USERNAME = "username";
    const string PASSWORD = "password";
    const string TITLE = "title";
    const string INGREDIENTS = "ingredients";
    const string STEPS = "steps";
    const string NAME = "name";
    const string AMOUNT = "amount";
    const string TYPE = "type";
    const string ACTION = "action";
    const string RECIPE = "recipe";
    const string INGREDIENT = "ingredient";
    const string COUNT = "count";
}

#endif
