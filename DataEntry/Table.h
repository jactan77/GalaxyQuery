#pragma once
#include <string>
#include <map>
#include <vector>
#include <format>
#include <iostream>
#ifndef TABLE_H
#define TABLE_H
struct Table{
    std::string name;
    std::map<int,std::pair<std::string, std::string>> columns; //  {name, dataType}
    std::map<int, std::vector<std::string>> rows;
    Table(std::string const& name, std::map<int,std::pair<std::string, std::string>> const& columns);

    auto isColumnExists(std::string const& name);
    auto addColumn(std::string const& columnName, std::string const& dataType) -> void;
    auto renameColumn(std::string const& columnName,std::string const& newName)->void;
};






#endif
