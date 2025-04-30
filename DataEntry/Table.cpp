

#include "Table.h"




auto Table::isColumnExists(std::string const &name) {
    return std::ranges::find_if(columns,[&](auto const& column)->bool {
       return column->getName() == name;
    });
}

auto Table::addColumn(std::string const& columnName, std::string const& dataType) -> void {
    if (isColumnExists(columnName) != this->columns.end()) {
        throw std::runtime_error(std::format("Cannot add the {}: The column name is already taken.",columnName));
    }
    columns.push_back(new Column(columnName,dataType));
}

auto Table::renameColumn(std::string const& columnName,std::string const &newName) -> void {
    const auto it = isColumnExists(columnName);
    if (it != columns.end()) {
            if (isColumnExists(newName) != columns.end()) {
                throw std::runtime_error("Cannot rename the column: The column name is already taken.");
            }
        (*it)->setName(newName);
        return;
    }
    throw std::runtime_error(std::format("No column with the name {} was found.",columnName));

}
 auto Table::insertValues(std::map<std::string, std::string> const& values)->void { // The key represents the column name, and the value represents the value retrieved from the query.
        for (auto const& [columnName,value]:values) {
            auto getColumn = isColumnExists(columnName);
            if (getColumn != columns.end()) {
                (*getColumn)->insertValue(this->id,value);
                continue;
            }
            throw std::runtime_error(std::format("No column with the name {} was found.",columnName));
        }
        ++id;
}

auto Table::clearRows() -> void {
    for (auto& column: columns) {
        column->eraseFieldValues();
    }
    std::cout << std::format("All rows from the table {} have been deleted.", this->name) << std::endl;
}
auto Table::setColumns(std::map<std::string,std::string> const& columnData)->std::vector<Column*> {
    auto columns = std::vector<Column*>();
    for (auto const& [columnName,dataType]:columnData) {
        columns.push_back(new Column(columnName,dataType));
    }
    return columns;
}