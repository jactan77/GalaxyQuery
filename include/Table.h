#pragma once
#include "Column.h"
#include <string>
#include <map>
#include <utility>
#include <iostream>
#include <vector>
#include <format>
#include <algorithm>
#include <set>
#include <regex>

struct Table {
    std::string name;
    std::vector<Column*> columns;
    Table(std::string  name, std::vector<std::pair<std::string,std::string>> const& columns):name(std::move(name)),columns(setColumns(columns)),id(1){};
    Table(std::string  name, std::vector<Column*> const& columns, const int id):name(std::move(name)),columns(columns),id(id){}
    ~Table()=default;
    [[nodiscard]] auto getID() const-> int;
    [[nodiscard]] auto getColumns() const -> std::vector<Column*> {return columns;}
    [[nodiscard]] auto getTableName() const->std::string {return name;}
    auto setColumns(std::vector<Column*> const& columns)->void;
    auto setId(int id)->void;
    auto addColumn(std::string const& columnName, std::string const& dataType) -> void;
    auto renameColumn(std::string const& columnName,std::string const& newName)->void;
    auto updateValues(std::map<std::string,std::string> const& values, std::vector<std::string> const& conditions)->void;
    auto dropColumn(std::string const& columnName)->void;
    auto insertValues(std::map<std::string, std::string> const& values)->void;
    auto clearRows() -> void;
    auto selectColumns(std::vector<std::string> const& selectedColumns)-> void;
    auto selectFilteredColumns(std::vector<std::string> const& selectedColumns, std::vector<std::string> const& conditions) -> void;
    auto selectOrderedColumns(std::vector<std::string> const& selectedColumns,std::string const& orderByColumn)->void;
private:
    int id;
    auto isColumnExists(std::string const& name);
    [[nodiscard]] auto processConditions(std::vector<std::string> const &conditions)  -> std::set<int>;
    static auto setColumns(std::vector<std::pair<std::string,std::string>> const& columnData)->std::vector<Column*>;
    template <std::ranges::range T>
    static auto printTable(std::vector<Column *> const &columnsToPrint, T const &ids)-> std::string;
};







