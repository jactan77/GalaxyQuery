#include "DB.h"

Db::Db(std::string const &name) : name(name) {}

Db::Db() : name("null") {}

auto Db::addTable(std::unique_ptr<Table> t) -> void {
  tables.push_back(std::move(t));
}

auto Db::findTable(std::string const &tableName) -> Table * {
  auto it = std::ranges::find_if(
      tables, [&](auto const &t) { return t->name == tableName; });
  return it != tables.end() ? it->get() : nullptr;
}

auto Db::processCreateTable(
    std::string const &tableName,
    std::vector<std::pair<std::string, std::string>> const &columns) -> void {
  if (findTable(tableName)) {
    throw std::runtime_error(std::format(
        "Cannot add a table named {}: That name is already taken.", tableName));
  }
  tables.push_back(std::make_unique<Table>(tableName, columns));
  std::cout << "You have created in DB: " << name
            << " a table named : " << tableName << std::endl;
}

auto Db::processInsert(std::string const &tableName,
                       std::map<std::string, std::string> const &values)
    -> void {
  auto *table = findTable(tableName);
  if (!table) {
    throw std::runtime_error(
        std::format("No table with the name {} was found.", tableName));
  }
  table->insertValues(values);
}

auto Db::processUpdate(std::string const &tableName,
                       std::map<std::string, std::string> const &values,
                       std::vector<std::string> const &conditions) -> void {
  auto *table = findTable(tableName);
  if (!table) {
    throw std::runtime_error(
        std::format("No table with the name {} was found.", tableName));
  }
  table->updateValues(values, conditions);
}

auto Db::processDelete(std::string const &tableName) -> void {
  auto *table = findTable(tableName);
  if (!table) {
    throw std::runtime_error(
        std::format("No table with the name {} was found.", tableName));
  }
  table->clearRows();
}

auto Db::processSelect(std::string const &tableName,
                       std::vector<std::string> const &columns) -> void {
  auto *table = findTable(tableName);
  if (!table) {
    throw std::runtime_error(
        std::format("No table with the name {} was found.", tableName));
  }
  table->selectColumns(columns);
}

auto Db::processSelect(std::string const &tableName,
                       std::vector<std::string> const &columns,
                       std::vector<std::string> const &conditions) -> void {
  auto *table = findTable(tableName);
  if (!table) {
    throw std::runtime_error(
        std::format("No table with the name {} was found.", tableName));
  }
  table->selectFilteredColumns(columns, conditions);
}

auto Db::processOrderSelect(std::string const &tableName,
                            std::vector<std::string> const &pickedColumns,
                            std::string const &byColumnName) -> void {
  auto *table = findTable(tableName);
  if (!table) {
    throw std::runtime_error(
        std::format("No table with the name {} was found.", tableName));
  }
  table->selectOrderedColumns(pickedColumns, byColumnName);
}

auto Db::processAlterRename(std::string const &tableName,
                            std::string const &columnName,
                            std::string const &newColumnName) -> void {
  auto *table = findTable(tableName);
  if (!table) {
    throw std::runtime_error(
        std::format("No table with the name {} was found.", tableName));
  }
  table->renameColumn(columnName, newColumnName);
}

auto Db::processAlterAdd(std::string const &tableName,
                         std::string const &columnName,
                         std::string const &dataType) -> void {
  auto *table = findTable(tableName);
  if (!table) {
    throw std::runtime_error(
        std::format("No table with the name {} was found.", tableName));
  }
  table->addColumn(columnName, dataType);
}

auto Db::processAlterDelete(std::string const &tableName,
                            std::string const &columnName) -> void {
  auto *table = findTable(tableName);
  if (!table) {
    throw std::runtime_error(
        std::format("No table with the name {} was found.", tableName));
  }
  table->dropColumn(columnName);
}

auto Db::processTableDrop(std::string const &tableName) -> void {
  auto it = std::ranges::find_if(
      tables, [&](auto const &t) { return t->name == tableName; });
  if (it == tables.end()) {
    throw std::runtime_error(
        std::format("No table with the name {} was found.", tableName));
  }
  tables.erase(it);
  std::cout << "Our table named " << tableName
            << " has been successfully deleted." << std::endl;
}
