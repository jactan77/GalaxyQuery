

#include "Table.h"
#include <random>


auto Table::isColumnExists(std::string const &name) {
    return std::ranges::find_if(columns,[&](auto const& column)->bool {
       return column->getName() == name;
    });
}
auto Table::getID() const-> int {
    return id;
};
auto Table::addColumn(std::string const& columnName, std::string const& dataType) -> void {
    if (isColumnExists(columnName) != this->columns.end()) {
        throw std::runtime_error(std::format("Cannot add the {}: The column name is already taken.",columnName));
    }
    auto* newColumn = new Column(columnName,dataType);
    columns.push_back(newColumn);
    if (this->id > 1) {
        newColumn->insertDefaultValues(this->id);
    }
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
auto Table::processConditions(std::vector<std::string> const& conditions) -> std::set<int> {
    std::set<int> matchingRows;
    bool isFirstCondition = true;
    std::string logicalOp = "AND";
    int i = 0;
    while (i < conditions.size()) {
        auto const& columnName = conditions[i++];
        auto const& operand  = conditions[i++];
        auto const& value = conditions[i++];

        auto findColumn  = isColumnExists(columnName);
        if (findColumn == columns.end()) {
            throw std::runtime_error(std::format("No column with the name {} was found.",columnName));
        }
        std::vector<int> currentRows = (*findColumn)->getFilteredRows(value,operand);
        if (isFirstCondition) {
            matchingRows = std::set(currentRows.begin(),currentRows.end());
            isFirstCondition = false;
        } else {
            if (logicalOp == "AND") {
                std::set<int> uniqueRows;
                for (int id: matchingRows) {
                    if (std::ranges::find(currentRows,id) != currentRows.end()) {
                        uniqueRows.insert(id);
                    }
                }
                matchingRows = uniqueRows;
            }
            if (logicalOp == "OR") {
                matchingRows.insert(currentRows.begin(), currentRows.end());
            }
        }
        if (i < conditions.size()) {
            logicalOp = conditions[i++];
        }
    }
    return matchingRows;
}
auto Table::printTable(std::vector<Column*> const& columnsToPrint,std::set<int> const& ids)-> std::string {
    std::stringstream result;
    std::vector<std::vector<std::string>> columnLines;
    std::vector<size_t> columnWidths;

    for (const auto& column : columnsToPrint) {
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
    for (const auto& lines : columnLines) {
        maxRows = std::max(maxRows, lines.size());
    }

    for (size_t rowIndex = 0; rowIndex < maxRows; ++rowIndex) {
        for (size_t colIndex = 0; colIndex < columnLines.size(); ++colIndex) {
            const auto& lines = columnLines[colIndex];
            result << lines[rowIndex];
            if (colIndex < columnLines.size() - 1) {
                result << " ";
            }
        }
        result << "\n";
    }
    return result.str();
}
auto Table::selectColumns(std::vector<std::string> const& selectedColumns) -> void {
    std::vector<Column*> columnsToPrint;
    if (selectedColumns.size() == 1 && selectedColumns[0] == "*") {
        columnsToPrint = this->columns;
    } else {
        for (const auto& columnName : selectedColumns) {
            auto columnIt = isColumnExists(columnName);
            if (columnIt == this->columns.end()) {
                throw std::runtime_error(std::format("No column with the name {} was found.", columnName));
            }
            columnsToPrint.push_back(*columnIt);
        }
    }
    std::set<int> allIds;
    for (int i = 1; i <= this->id; ++i) {
        allIds.insert(i);
    }
    std::cout << printTable(columnsToPrint,allIds) << std::endl;
}

auto Table::selectFilteredColumns(std::vector<std::string> const& selectedColumns, std::vector<std::string> const& conditions) -> void {
    std::vector<Column*> columnsToPrint;
    if (selectedColumns.size() == 1 && selectedColumns[0] == "*") {
        columnsToPrint = this->columns;
    } else {
        for (const auto& columnName : selectedColumns) {
            auto columnIt = isColumnExists(columnName);
            if (columnIt == this->columns.end()) {
                throw std::runtime_error(std::format("No column with the name {} was found.", columnName));
            }
            columnsToPrint.push_back(*columnIt);
        }
    }
    const std::set<int> getFilteredIds = this->processConditions(conditions);
    std::cout << printTable(columnsToPrint,getFilteredIds);

}
 auto Table::insertValues(std::map<std::string, std::string> const& values)->void {
        int const getID = this->id;
        if (values.size() != this->columns.size()) {
            throw std::runtime_error("Missing values for required columns");
        }
       for (auto const& [columnName,value]:values) {
            auto getColumn = isColumnExists(columnName);
            if(getColumn == columns.end()) {
                throw std::runtime_error(std::format("No column with the name {} was found.",columnName));
            }
            if (Column::selectType(value) != (*getColumn)->getDataType()) {
               throw std::runtime_error(std::format("The provided value {} is incompatible with the data type of the column {}.",value,columnName));
           }
        }
        for (auto const& [columnName,value]:values) {
            auto  getColumn = isColumnExists(columnName);
                (*getColumn)->insertValue(getID,value);
        }
        std::cout << std::format("INSERT operation completed. Record ID: {}.",this->id++);


}

auto Table::clearRows() -> void {
    for (auto& column: columns) {
        column->eraseFieldValues();
    }
    std::cout << std::format("All rows from the table {} have been deleted.", this->name) << std::endl;
}
auto Table::setColumns(std::vector<std::pair<std::string,std::string>> const& columnData)->std::vector<Column*> {
    auto columns = std::vector<Column*>();
    for (auto const& [columnName,dataType]:columnData) {
        columns.push_back(new Column(columnName,dataType));
    }
    return columns;
}
auto Table::setColumns(std::vector<Column*> const& columns)->void {
    this->columns=columns;
}
auto Table::setId(const int id)->void {
    this->id = id;
}
auto Table::dropColumn(std::string const& columnName)->void {
    auto const& getColumn = isColumnExists(columnName);
    if (getColumn != this->columns.end()) {
        delete *getColumn;
        this->columns.erase(getColumn);
        return;
    }
    throw std::runtime_error(std::format("No column with the name {} was found.", columnName));

}
