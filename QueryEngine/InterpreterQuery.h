#pragma once
#include <iostream>
#include <algorithm>
#include <functional>
#include <ranges>
#include "../DataEntry/DB.h"
#include <string>
#include <vector>
#include <map>
#include <regex>
#ifndef INTERPRETERQUERY_H
#define INTERPRETERQUERY_H



class InterpreterQuery {
public:
    static auto processQuery(Db*& db,std::string const& input)->void;
    static auto getTokens(std::string element)->std::vector<std::string>;
    static auto initializeCreate(std::string createStatement) -> bool;
    static inline auto const GalaxyKeywords = std::map<std::string, std::function<void(Db*, const std::vector<std::string>&)>>{
                {"SELECT", [](Db* db, const std::vector<std::string>& query) -> void { db->processSelect(query); }},
                {"INSERT", [](Db* db, const std::vector<std::string>& query) -> void { db->processInsert(query); }},
                {"UPDATE", [](Db* db, const std::vector<std::string>& query) -> void { db->processUpdate(query); }},
                {"CREATE", [](Db* db, const std::vector<std::string>& query) -> void { db->processCreate(query); }},
                {"DROP",   [](Db* db, const std::vector<std::string>& query) -> void { db->processDrop(query);   }},
                {"ALTER",  [](Db* db, const std::vector<std::string>& query) -> void { db->processAlter(query);  }},
                {"DELETE", [](Db* db, const std::vector<std::string>& query) -> void { db->processDelete(query); }}
    };
};




#endif //INTERPRETERQUERY_H
