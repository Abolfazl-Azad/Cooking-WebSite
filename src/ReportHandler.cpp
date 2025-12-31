#include "ReportHandler.hpp"

ReportHandler::ReportHandler(vector<Recipe>& recipes)
    : allRecipes(recipes) {}

string ReportHandler::handleReport(User* currentUser, map<string, string>& args) {
    if (args.find(ArgKeys::TYPE) == args.end()) {
        return Messages::BAD_REQUEST;
    }

    if (args[ArgKeys::TYPE] == ReportTypes::REFRIGERATOR_STATUS) {
        return handleRefrigeratorReport(currentUser);
    }

    if (args[ArgKeys::TYPE] == ReportTypes::MOST_LIKED) {
        return handleMostLikedReport(currentUser, args);
    }

    return Messages::BAD_REQUEST;
}

string ReportHandler::handleRefrigeratorReport(User* currentUser) {
    auto shelfMap = currentUser->getShelf();

    if (shelfMap.empty()) {
        return Messages::NO_ITEMS_IN_FRIDGE;
    }

    vector<pair<string, int>> shelfVec;
    for (auto const& item : shelfMap) {
        shelfVec.push_back(item);
    }

    sort(shelfVec.begin(), shelfVec.end(),
         [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.first > b.first;
    });

    string output = OutputFormats::FRIDGE_HEADER + currentUser->getUsername() + ":\n";

    for (size_t i = 0; i < shelfVec.size(); ++i) {
        output += shelfVec[i].first + ":" + to_string(shelfVec[i].second);
        if (i != shelfVec.size() - 1) {
            output += "\n";
        }
    }

    return output;
}

string ReportHandler::handleMostLikedReport(User* currentUser, map<string, string>& args) {
    if (currentUser->getRole() != CHEF) {
        return Messages::PERMISSION_DENIED;
    }

    if (allRecipes.empty()) {
        return Messages::EMPTY;
    }

    int countLimit = -1;
    if (args.find(ArgKeys::COUNT) != args.end()) {
        try {
            countLimit = stoi(args[ArgKeys::COUNT]);
        } catch (...) {
            return Messages::BAD_REQUEST;
        }
        if (countLimit <= 0) {
            return Messages::BAD_REQUEST;
        }
    }

    vector<Recipe> ownedRecipes;
    for (const auto& recipe : allRecipes) {
        if (recipe.getOwner() == currentUser->getUsername()) {
            ownedRecipes.push_back(recipe);
        }
    }

    if (ownedRecipes.empty()) {
        return Messages::CHEF_HAS_NO_RECIPE;
    }

    sort(ownedRecipes.begin(), ownedRecipes.end(),
         [](const Recipe& a, const Recipe& b) {
        if (a.getLikes() != b.getLikes()) {
            return a.getLikes() > b.getLikes();
        }
        return a.getName() > b.getName();
    });

    int limit = static_cast<int>(ownedRecipes.size());
    if (countLimit > 0 && countLimit < limit) {
        limit = countLimit;
    }

    string output;
    for (int i = 0; i < limit; ++i) {
        output += to_string(i + 1) + ". " + ownedRecipes[i].getName() + ": " +
                  to_string(ownedRecipes[i].getLikes()) + " likes";
        if (i != limit - 1) {
            output += "\n";
        }
    }

    return output;
}
