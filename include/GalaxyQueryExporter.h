#pragma once
#include "DB.h"
#include "Table.h"
#include "Column.h"
#include <sstream>
#include <fstream>

class GalaxyQueryExporter {
public:
    auto static saveToFile(Db*& db) -> void;
    auto static loadDb()->Db*;
    auto static parseName(const std::string& line,std::regex const& pattern)->std::string;
    auto static parseData(Db*& db,std::string line)->void; // Add existing tables and columns with values.
    auto static parseValues(const std::string &line)->std::map<int,std::string>;
};