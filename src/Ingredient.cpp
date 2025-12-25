#include "Ingredient.hpp"

Ingredient::Ingredient(const string& name,
                       const string& category,
                       int price)
    : name(name), category(category), price(price) {}

const string& Ingredient::getName() const {
    return name;
}

const string& Ingredient::getCategory() const {
    return category;
}

int Ingredient::getPrice() const {
    return price;
}
