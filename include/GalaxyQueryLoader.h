#pragma once
#include "DB.h"
#include "Table.h"
#include "Column.h"
#include <sstream>
#include <fstream>
#include <filesystem>

class GalaxyQueryLoader {
    auto static parseData(Db*& db,std::string line)->void;
    auto static parseValues(const std::string &line)->std::map<int,std::string>;
    auto static parseName(const std::string& line,std::regex const& pattern)->std::string;
public:
    auto static loadDb()->Db*;
};
