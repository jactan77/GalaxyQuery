#pragma once
#include "Column.h"
#include "DB.h"
#include "Table.h"
#include <filesystem>
#include <fstream>
#include <sstream>

class GalaxyQueryExporter {
public:
  static auto saveToFile(const Db &db) -> void;
  static auto saveToFile(std::nullptr_t) -> void;
};