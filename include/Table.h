#pragma once
#include "Column.h"
#include <algorithm>
#include <format>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

class Table {
public:
  std::string name;

  Table(std::string name,
        std::vector<std::pair<std::string, std::string>> const &columnDefs);
  Table(std::string name, std::vector<std::unique_ptr<Column>> cols, int id);
  ~Table() = default;

  Table(const Table &) = delete;
  Table &operator=(const Table &) = delete;
  Table(Table &&) = default;
  Table &operator=(Table &&) = default;

  [[nodiscard]] auto getID() const noexcept -> int { return id; }
  [[nodiscard]] auto getColumns() const noexcept
      -> const std::vector<std::unique_ptr<Column>> & {
    return columns;
  }
  [[nodiscard]] auto getTableName() const noexcept -> const std::string & {
    return name;
  }

  auto addColumn(std::string const &columnName, std::string const &dataType)
      -> void;
  auto renameColumn(std::string const &columnName, std::string const &newName)
      -> void;
  auto updateValues(std::map<std::string, std::string> const &values,
                    std::vector<std::string> const &conditions) -> void;
  auto dropColumn(std::string const &columnName) -> void;
  auto insertValues(std::map<std::string, std::string> const &values) -> void;
  auto clearRows() -> void;
  auto selectColumns(std::vector<std::string> const &selectedColumns) -> void;
  auto selectFilteredColumns(std::vector<std::string> const &selectedColumns,
                             std::vector<std::string> const &conditions)
      -> void;
  auto selectOrderedColumns(std::vector<std::string> const &selectedColumns,
                            std::string const &orderByColumn) -> void;

private:
  std::vector<std::unique_ptr<Column>> columns;
  int id;

  auto findColumn(std::string const &name) -> Column *;
  [[nodiscard]] auto findColumn(std::string const &name) const
      -> const Column *;
  [[nodiscard]] auto
  processConditions(std::vector<std::string> const &conditions)
      -> std::set<int>;

  template <std::ranges::range T>
  static auto
  printTable(const std::vector<std::unique_ptr<Column>> &columnsToPrint,
             T const &ids) -> std::string;
};
