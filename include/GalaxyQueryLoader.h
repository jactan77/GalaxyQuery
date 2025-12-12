#pragma once
#include "Column.h"
#include "DB.h"
#include "Table.h"
#include <filesystem>
#include <fstream>
#include <sstream>

class GalaxyQueryLoader {
public:
  static auto loadDb() -> std::unique_ptr<Db>;

private:
  static auto parseData(Db &db, std::string line) -> void;
  static auto parseValues(const std::string &line)
      -> std::map<int, std::string>;
  static auto parseName(const std::string &line, std::regex const &pattern)
      -> std::string;
};
