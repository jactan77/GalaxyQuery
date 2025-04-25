

#ifndef DB_H
#define DB_H
#pragma once
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <vector>
#include <regex>
#include "Table.h"

class  Db {
        std::string name;
        std::vector<Table> tables;
    public:
        explicit Db(std::string const& name);
        explicit Db();
        auto setTable(Table const& t)->void;
        auto getName()->std::string;
        auto processCreate(std::vector<std::string> createQuery)-> void;
        auto processInsert(std::vector<std::string> insertQuery)-> void;
        auto processUpdate(std::vector<std::string> updateQuery)-> void;
        auto processDelete(std::vector<std::string> deleteQuery)-> void;
        auto processSelect(std::vector<std::string> selectQuery)-> void;
        auto processAlter(std::vector<std::string> alterQuery)-> void;
        auto processDrop(std::vector<std::string> dropQuery)-> void;
        static inline auto dataTypes = std::vector<std::string>{
            "INT","STRING", "CHAR", "BOOL"
};


};





#endif