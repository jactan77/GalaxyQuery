#pragma once
#include "Table.h"
#include <algorithm>
#include <format>
#include <iostream>
#include <map>
#include <memory>
#include <ranges>
#include <string>
#include <vector>

class Db {
public:
  explicit Db(std::string const &name);
  Db();
  ~Db() = default;

  Db(const Db &) = delete;
  Db &operator=(const Db &) = delete;
  Db(Db &&) = default;
  Db &operator=(Db &&) = default;

  auto addTable(std::unique_ptr<Table> t) -> void;
  [[nodiscard]] auto getDbName() const noexcept -> const std::string & {
    return name;
  }
  [[nodiscard]] auto getTables() const noexcept
      -> const std::vector<std::unique_ptr<Table>> & {
    return tables;
  }
  [[nodiscard]] auto getTables() noexcept
      -> std::vector<std::unique_ptr<Table>> & {
    return tables;
  }

  auto processCreateTable(
      std::string const &tableName,
      std::vector<std::pair<std::string, std::string>> const &columns) -> void;
  auto processInsert(std::string const &tableName,
                     std::map<std::string, std::string> const &values) -> void;
  auto processUpdate(std::string const &tableName,
                     std::map<std::string, std::string> const &values,
                     std::vector<std::string> const &conditions) -> void;
  auto processDelete(std::string const &tableName) -> void;
  auto processAlterRename(std::string const &tableName,
                          std::string const &columnName,
                          std::string const &newColumnName) -> void;
  auto processAlterAdd(std::string const &tableName,
                       std::string const &columnName,
                       std::string const &dataType) -> void;
  auto processAlterDelete(std::string const &tableName,
                          std::string const &columnName) -> void;
  auto processSelect(std::string const &tableName,
                     std::vector<std::string> const &columns) -> void;
  auto processSelect(std::string const &tableName,
                     std::vector<std::string> const &columns,
                     std::vector<std::string> const &conditions) -> void;
  auto processOrderSelect(std::string const &tableName,
                          std::vector<std::string> const &pickedColumns,
                          std::string const &byColumnName) -> void;
  auto processTableDrop(std::string const &tableName) -> void;

private:
  std::string name;
  std::vector<std::unique_ptr<Table>> tables;

  auto findTable(std::string const &tableName) -> Table *;
};
