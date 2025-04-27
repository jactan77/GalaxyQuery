

#include "Table.h"



Table::Table(std::string const& name, std::map<int,std::pair<std::string, std::string>> const& columns){
             this->name = name;
             this->columns = columns;
             this->rows = std::map<int, std::vector<std::string>>();
}
auto Table::isColumnExists(std::string const &name) {
    return std::ranges::find_if(columns,[&](auto const& pair)->bool { // pair -> {id, {name, dataType}}
       auto getColumn =  pair.second;
       return getColumn.first == name;
    });
}

auto Table::addColumn(std::string const& columnName, std::string const& dataType) -> void {
    const auto it =  isColumnExists(columnName);
    if (it != this->columns.end()) {
        throw std::runtime_error("Cannot add the column: The column name is already taken.");
    }
    int index = (this->columns.rbegin())->first;
    std::cout << "Index: " << index << std::endl;
    this->columns[++index] = {columnName,dataType};
    std::cout << "Size: " << this->columns.size() << std::endl;
}

auto Table::renameColumn(std::string const& columnName,std::string const &newName) -> void {
    const auto it = isColumnExists(columnName);
    if (it != columns.end()) {
        auto& getIndex = it->first;
        auto&[name, dataType] = it->second;
        name = newName;
        this->columns[getIndex] = {name, dataType};
        return;
    }
    throw std::runtime_error(std::format("No column with the name {} was found.",columnName));

}

