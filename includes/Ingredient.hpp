#ifndef INGREDIENT_HPP
#define INGREDIENT_HPP

#include "global.hpp"

class Ingredient {
private:
    string name;
    string category;
    int price;

public:
    Ingredient(const string& name,
               const string& category,
               int price);

    const string& getName() const;
    const string& getCategory() const;
    int getPrice() const;
};

#endif
