
#include <iostream>
#include <ranges>
#include <string>
#include <algorithm>
#include <vector>
#include "../QueryTokens/DDL.h"
#ifndef INTERPRETERQUERY_H
#define INTERPRETERQUERY_H



class InterpreterQuery {
    public:
            static auto processQuery(std::string input)->Db;
            static auto getTokens(std::string element)->std::vector<std::string>;
            static inline auto GalaxyKeywords = std::vector<std::string>{
                "SELECT", "INSERT", "UPDATE", "DELETE",
                "CREATE", "ALTER", "DROP"
            };
};



#endif //INTERPRETERQUERY_H
