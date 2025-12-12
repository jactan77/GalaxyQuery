#include "Table.h"

Table::Table(std::string name,
             std::vector<std::pair<std::string, std::string>> const &columnDefs)
    : name(std::move(name)), id(1) {
  for (auto const &[colName, dataType] : columnDefs) {
    columns.push_back(std::make_unique<Column>(colName, dataType));
  }
}

Table::Table(std::string name, std::vector<std::unique_ptr<Column>> cols,
             int id)
    : name(std::move(name)), columns(std::move(cols)), id(id) {}

auto Table::findColumn(std::string const &colName) -> Column * {
  auto it = std::ranges::find_if(
      columns, [&](auto const &col) { return col->getName() == colName; });
  return it != columns.end() ? it->get() : nullptr;
}

auto Table::findColumn(std::string const &colName) const -> const Column * {
  auto it = std::ranges::find_if(
      columns, [&](auto const &col) { return col->getName() == colName; });
  return it != columns.end() ? it->get() : nullptr;
}

auto Table::addColumn(std::string const &columnName,
                      std::string const &dataType) -> void {
  if (findColumn(columnName)) {
    throw std::runtime_error(std::format(
        "Cannot add the {}: The column name is already taken.", columnName));
  }
  auto newColumn = std::make_unique<Column>(columnName, dataType);
  if (id > 1) {
    newColumn->insertDefaultValues(id);
  }
  columns.push_back(std::move(newColumn));
}

auto Table::renameColumn(std::string const &columnName,
                         std::string const &newName) -> void {
  auto *col = findColumn(columnName);
  if (!col) {
    throw std::runtime_error(
        std::format("No column with the name {} was found.", columnName));
  }
  if (findColumn(newName)) {
    throw std::runtime_error(
        "Cannot rename the column: The column name is already taken.");
  }
  col->setName(newName);
}

auto Table::updateValues(std::map<std::string, std::string> const &values,
                         std::vector<std::string> const &conditions) -> void {
  auto const &matchingRows = processConditions(conditions);
  for (auto const &[columnName, value] : values) {
    auto *col = findColumn(columnName);
    if (!col) {
      throw std::runtime_error(
          std::format("No column with the name {} was found.", columnName));
    }
    for (int rowId : matchingRows) {
      col->updateValue(rowId, value);
    }
  }
  std::cout << std::format("Updated {} rows in table {}", matchingRows.size(),
                           name)
            << std::endl;
}

auto Table::processConditions(std::vector<std::string> const &conditions)
    -> std::set<int> {
  std::set<int> matchingRows;
  bool isFirstCondition = true;
  std::string logicalOp = "AND";
  size_t i = 0;

  while (i < conditions.size()) {
    auto const &columnName = conditions[i++];
    auto const &operand = conditions[i++];
    auto const &value = conditions[i++];

    auto *col = findColumn(columnName);
    if (!col) {
      throw std::runtime_error(
          std::format("No column with the name {} was found.", columnName));
    }

    std::vector<int> currentRows = col->getFilteredRows(value, operand);

    if (isFirstCondition) {
      matchingRows = std::set(currentRows.begin(), currentRows.end());
      isFirstCondition = false;
    } else {
      if (logicalOp == "AND") {
        std::set<int> uniqueRows;
        for (int rowId : matchingRows) {
          if (std::ranges::find(currentRows, rowId) != currentRows.end()) {
            uniqueRows.insert(rowId);
          }
        }
        matchingRows = uniqueRows;
      } else if (logicalOp == "OR") {
        matchingRows.insert(currentRows.begin(), currentRows.end());
      }
    }

    if (i < conditions.size()) {
      logicalOp = conditions[i++];
    }
  }
  return matchingRows;
}

template <std::ranges::range T>
auto Table::printTable(
    const std::vector<std::unique_ptr<Column>> &columnsToPrint, T const &ids)
    -> std::string {
  std::stringstream result;
  std::vector<std::vector<std::string>> columnLines;
  std::vector<size_t> columnWidths;

  for (const auto &column : columnsToPrint) {
    std::stringstream colStream(column->printRows(ids));
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(colStream, line)) {
      lines.push_back(line);
    }
    columnLines.push_back(lines);
    columnWidths.push_back(column->calculateWidth());
  }

  size_t maxRows = 0;
  for (const auto &lines : columnLines) {
    maxRows = std::max(maxRows, lines.size());
  }

  for (size_t rowIndex = 0; rowIndex < maxRows; ++rowIndex) {
    for (size_t colIndex = 0; colIndex < columnLines.size(); ++colIndex) {
      result << columnLines[colIndex][rowIndex];
      if (colIndex < columnLines.size() - 1) {
        result << " ";
      }
    }
    result << "\n";
  }
  return result.str();
}

auto Table::selectColumns(std::vector<std::string> const &selectedColumns)
    -> void {
  std::vector<std::reference_wrapper<const std::unique_ptr<Column>>>
      columnsToPrint;

  if (selectedColumns.size() == 1 && selectedColumns[0] == "*") {
    for (const auto &col : columns) {
      columnsToPrint.push_back(std::cref(col));
    }
  } else {
    for (const auto &colName : selectedColumns) {
      auto it = std::ranges::find_if(
          columns, [&](auto const &c) { return c->getName() == colName; });
      if (it == columns.end()) {
        throw std::runtime_error(
            std::format("No column with the name {} was found.", colName));
      }
      columnsToPrint.push_back(std::cref(*it));
    }
  }

  std::set<int> allIds;
  for (int i = 1; i < id; ++i) {
    allIds.insert(i);
  }
  std::cout << printTable(columns, allIds) << std::endl;
}

auto Table::selectFilteredColumns(
    std::vector<std::string> const &selectedColumns,
    std::vector<std::string> const &conditions) -> void {
  const std::set<int> filteredIds = processConditions(conditions);
  std::cout << printTable(columns, filteredIds);
}

auto Table::selectOrderedColumns(
    std::vector<std::string> const &selectedColumns,
    std::string const &orderByColumn) -> void {
  auto *orderCol = findColumn(orderByColumn);
  if (!orderCol) {
    throw std::runtime_error(
        std::format("No column with the name {} was found.", orderByColumn));
  }
  const std::vector<int> orderedIds = orderCol->getOrderedRows();
  std::cout << printTable(columns, orderedIds);
}

auto Table::insertValues(std::map<std::string, std::string> const &values)
    -> void {
  if (values.size() != columns.size()) {
    throw std::runtime_error("Missing values for required columns");
  }

  for (auto const &[columnName, value] : values) {
    auto *col = findColumn(columnName);
    if (!col) {
      throw std::runtime_error(
          std::format("No column with the name {} was found.", columnName));
    }
    if (Column::selectType(value) != col->getDataType()) {
      throw std::runtime_error(
          std::format("The provided value {} is incompatible with the data "
                      "type of the column {}.",
                      value, columnName));
    }
  }

  for (auto const &[columnName, value] : values) {
    auto *col = findColumn(columnName);
    col->insertValue(id, value);
  }
  std::cout << std::format("INSERT operation completed. Record ID: {}.", id++);
}

auto Table::clearRows() -> void {
  for (auto &column : columns) {
    column->eraseFieldValues();
  }
  std::cout << std::format("All rows from the table {} have been deleted.",
                           name)
            << std::endl;
}

auto Table::dropColumn(std::string const &columnName) -> void {
  auto it = std::ranges::find_if(
      columns, [&](auto const &col) { return col->getName() == columnName; });
  if (it == columns.end()) {
    throw std::runtime_error(
        std::format("No column with the name {} was found.", columnName));
  }
  columns.erase(it);
}
