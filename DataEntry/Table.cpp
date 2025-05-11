

#include "Table.h"

#include <random>


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
auto Table::updateValues(std::map<std::string,std::string> const& values, std::vector<std::string> const& conditions)->void {
        auto const& getMatchingRows = this->processConditions(conditions);
        // set new Values
        for (auto const& [columnName, value]: values) {
                auto const& findColumn = isColumnExists(columnName);
                if (findColumn != this->columns.end()) {
                    for (int rowId : getMatchingRows) {
                        (*findColumn)->updateValue(rowId,value);
                    }

                }else {
                        throw std::runtime_error(std::format("No column with the name {} was found.",columnName));
                }
        }
    std::cout << std::format("Updated {} rows in table {}", getMatchingRows.size(), this->name) << std::endl;
}
auto Table::processConditions(std::vector<std::string> const& conditions) -> std::vector<int> {
    std::vector<int> matchingRows;
    bool isFirstCondition = true;


    int i = 0;
    while (i < conditions.size()) {
        auto const& columnName = conditions[i++];
        auto const& operand  = conditions[i++];
        auto const& value = conditions[i++];

        std::string logicalOp = "AND";
        if (i < conditions.size()) {
            logicalOp = conditions[i++];
        }
        auto findColumn  = isColumnExists(columnName);
        if (findColumn == columns.end()) {
            throw std::runtime_error(std::format("No column with the name {} was found.",columnName));

        }
        std::vector<int> currentRows = (*findColumn)->getFilteredRows(value,operand);
        if (isFirstCondition) {
            matchingRows = currentRows;
            isFirstCondition = false;
        } else {
            if (logicalOp == "AND") {
                std::vector<int> uniqueRows;
                for (int id: matchingRows) {
                    if (std::ranges::find(currentRows.begin(),currentRows.end(),id) != currentRows.end()) {
                        uniqueRows.push_back(id);
                    }
                }
                matchingRows = uniqueRows;
            }
            else if (logicalOp == "OR") {
                for (int id: currentRows) {
                    if (std::ranges::find(matchingRows.begin(),matchingRows.end(),id) == matchingRows.end()) {
                        matchingRows.push_back(id);
                    }
                }
            }
        }
    }
    return matchingRows;
}
auto Table::printColumns(std::vector<std::string> const& columns)-> void {
    if (columns.size() == 1 && columns.at(0) == "*") {
        for (Column *& column: this->columns) {
            std::cout << column->printAllRows() << "\t";
        }
    } else {
        for (auto const& columnName: columns) {
            auto getColumn = isColumnExists(columnName);
            if (getColumn != this->columns.end()) {
                std::cout << (*getColumn)->printAllRows() << "\t";
                continue;
            }
            throw std::runtime_error(std::format("No column with the name {} was found.",columnName));
        }
    }
}
auto Table::printFilteredColumns(std::vector<std::string> const& columns, std::vector<std::string> const& conditions) -> void {

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
