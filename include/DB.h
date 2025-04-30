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
    auto processUpdate(std::vector<std::string> const&updateQuery)-> void;
    auto processDelete(std::string const& tableName)-> void;
    auto processAlterRename(std::string const& tableName,std::string const& columnName, std::string const& newColumnName)-> void;
    auto processAlterAdd(std::string const& tableName,std::string const& columnName,std::string const& dataType)-> void;
    auto processSelect(std::vector<std::string> const& selectQuery)-> void;
    auto processTableDrop(std::string const& tableName)-> void;




};

