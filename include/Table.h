#pragma once
#include "Column.h"
#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <vector>
#include <format>
#include <algorithm>
#include <regex>

struct Table{
    std::string name;
    std::vector<Column*> columns; // Columns, each having its own field values.
    int id; // AUTO_INCREMENT as a PRIMARY_KEY
    Table(std::string  name, std::map<std::string,std::string> const& columns):name(std::move(name)),columns(setColumns(columns)),id(0){};
    ~Table()=default;
    auto addColumn(std::string const& columnName, std::string const& dataType) -> void;
    auto renameColumn(std::string const& columnName,std::string const& newName)->void;
    auto updateValues(std::map<std::string,std::string> const& values, std::vector<std::string> const& conditions)->void;
    auto insertValues(std::map<std::string, std::string> const& values)->void; // The key represents the column name, and the value represents the value retrieved from the query.
    auto clearRows() -> void;
    auto printColumns(std::vector<std::string> const& columns)-> void; // select without where
    auto printFilteredColumns(std::vector<std::string> const& columns, std::vector<std::string> const& conditions) -> void; //  select with where
private:
    auto isColumnExists(std::string const& name);
    [[nodiscard]] auto processConditions(std::vector<std::string> const &conditions)  -> std::vector<int>;
    static auto setColumns(std::map<std::string,std::string> const& columnData)->std::vector<Column*>; // The key represents the column name, and the value represents the dataType.
};







