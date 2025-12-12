#include "GalaxyQueryLoader.h"

auto GalaxyQueryLoader::loadDb() -> std::unique_ptr<Db> {
  std::fstream file("../data.txt", std::ios::in);
  std::string line;

  if (!file || std::filesystem::file_size("../data.txt") == 0) {
    return nullptr;
  }

  std::getline(file, line);
  auto dbName = parseName(line, std::regex(R"(^DB:(\w+)$)"));
  auto db = std::make_unique<Db>(dbName);

  while (std::getline(file, line)) {
    if (!line.empty()) {
      parseData(*db, line);
    }
  }
  return db;
}

auto GalaxyQueryLoader::parseName(const std::string &line,
                                  std::regex const &pattern) -> std::string {
  std::smatch match;
  if (std::regex_search(line, match, pattern)) {
    return match[1];
  }
  return {};
}

auto GalaxyQueryLoader::parseData(Db &db, std::string line) -> void {
  auto tableName = parseName(line, std::regex(R"(^(\w+):)"));
  line = std::regex_replace(line, std::regex(R"(^(\w+):)"), "");

  auto parseColumns = std::ranges::views::split(line, ':') |
                      std::ranges::views::transform([](auto const &subrange) {
                        return std::string(subrange.begin(), subrange.end());
                      });

  auto columns = std::vector<std::unique_ptr<Column>>();
  size_t maxId = 1;

  for (auto subrange : parseColumns) {
    auto columnName = parseName(subrange, std::regex(R"(^(\w+)\()"));
    auto dataType = parseName(subrange, std::regex(R"(^\w+\((\w+)\))"));
    auto column = std::make_unique<Column>(columnName, dataType);
    auto fieldValues = parseValues(subrange);
    maxId = std::max(maxId, fieldValues.size());
    column->setFieldValues(fieldValues);
    columns.push_back(std::move(column));
  }

  if (!columns.empty()) {
    auto table = std::make_unique<Table>(tableName, std::move(columns),
                                         static_cast<int>(maxId + 1));
    db.addTable(std::move(table));
  }
}

auto GalaxyQueryLoader::parseValues(const std::string &line)
    -> std::map<int, std::string> {
  auto values = parseName(line, std::regex(R"(^\w+\(\w+\)\{([^}]+)\})"));
  auto parseVals = std::ranges::views::split(values, ',') |
                   std::ranges::views::transform([](auto const &subrange) {
                     return std::string(subrange.begin(), subrange.end());
                   });

  auto fieldValues = std::map<int, std::string>();
  int id = 1;
  for (auto it = parseVals.begin(); it != parseVals.end(); ++it, ++id) {
    fieldValues.insert({id, *it});
  }
  return fieldValues;
}