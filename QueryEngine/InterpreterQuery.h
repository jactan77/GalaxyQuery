
#include <iostream>
#include <algorithm>
#include <ranges>
#include "../DataEntry/DB.h"
#include <string>
#include <vector>
#include <regex>
#include <map>
#ifndef INTERPRETERQUERY_H
#define INTERPRETERQUERY_H



class InterpreterQuery {
public:
    static auto processQuery(std::string input)->void;
    static auto getTokens(std::string element)->std::vector<std::string>;
    static auto initializeCreate(std::string createStatement) -> bool;
    static inline auto GalaxyKeywords = std::vector<std::string> {
        "SELECT", "INSERT", "UPDATE", "DELETE",
          "CREATE", "ALTER", "DROP"
    };
};



#endif //INTERPRETERQUERY_H
