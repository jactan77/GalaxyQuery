

#ifndef DB_H
#define DB_H
#pragma once
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <format>

#include <vector>
#include <regex>
#include "Table.h"

class  Db {
        std::string name;
        std::vector<Table*> tables;

    public:
        explicit Db(std::string const& name);
        explicit Db();

        auto setTable(Table* t)->void;
        auto getDbName()->std::string;
        auto cleanTables()->void;

        auto tableExists(std::string const &tableName);

        static auto getDataType(std::string const& value)->std::string;


        auto processCreate(std::vector<std::string> const&createQuery)-> void;
        auto processInsert(std::vector<std::string> const&insertQuery)-> void;
        auto processUpdate(std::vector<std::string> const&updateQuery)-> void;
        auto processDelete(std::vector<std::string> const& deleteQuery)-> void;
        auto processSelect(std::vector<std::string> const& selectQuery)-> void;
        auto processAlter(std::vector<std::string>  const& alterQuery)-> void;
        auto processDrop(std::vector<std::string>   const& dropQuery)-> void;

        static inline auto dataTypes = std::vector<std::string>{
            "INT","STRING", "CHAR", "BOOL"
};


};





#endif