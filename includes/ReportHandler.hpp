#ifndef REPORT_HANDLER_HPP
#define REPORT_HANDLER_HPP

#include "global.hpp"
#include "Recipe.hpp"
#include "User.hpp"

class ReportHandler {
private:
    vector<Recipe>& allRecipes;

    string handleRefrigeratorReport(User* currentUser);
    string handleMostLikedReport(User* currentUser, map<string, string>& args);

public:
    ReportHandler(vector<Recipe>& recipes);
    string handleReport(User* currentUser, map<string, string>& args);
};

#endif
