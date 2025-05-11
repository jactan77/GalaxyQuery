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





    auto processCreateTable(std::string const &tableName, std::map<std::string, std::string> const &columns) -> void;
    auto processInsert(std::string const& tableName,std::map<std::string, std::string> const& values)-> void;
    auto processUpdate(std::string const& tableName, std::map<std::string,std::string> const& values, std::vector<std::string> const& conditions )-> void;
    auto processDelete(std::string const& tableName)-> void;
    auto processAlterRename(std::string const& tableName,std::string const& columnName, std::string const& newColumnName)-> void;
    auto processAlterAdd(std::string const& tableName,std::string const& columnName,std::string const& dataType)-> void;
    auto processSelect(std::string const& tableName,std::vector<std::string> const& columns)-> void;
    auto processSelect(std::string const& tableName,std::vector<std::string> const& columns,std::vector<std::string> const& conditions)-> void;

    auto processTableDrop(std::string const& tableName)-> void;



};

