#pragma once
#include "DB.h"
#include "Table.h"
#include "Column.h"
#include <sstream>
#include <fstream>
#include <filesystem>
class GalaxyQueryExporter {
public:
    auto static saveToFile(Db*& db) -> void;
};