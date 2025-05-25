

#include "DB.h"




Db::Db(std::string const& name){
    this->name = name;
    this->tables = std::vector<Table*>();
}

Db::Db() {
    this->name = std::string("null");
    this->tables = std::vector<Table*>();
}
auto Db::setTable(Table* const t) -> void {
    this->tables.push_back(t);
}
auto Db::getDbName() -> std::string {
    return this->name;
}
auto Db::cleanTables()  ->void {
    for (auto const* table : this->tables) {
            delete table;
    }
    this->tables.clear();
}
auto Db::tableExists(std::string const& tableName)   {
    return std::ranges::find_if(this->tables,[&](const Table* t)-> bool{
          return t->name == tableName;
      });
}



auto Db::processCreateTable(std::string const& tableName, std::vector<std::pair<std::string,std::string>> const& columns)-> void {
        if (tableExists(tableName) == this->tables.end()) {
                this->setTable(new Table(tableName,columns));
                std::cout << "You have created in DB: " << this->name << " a table named : " << tableName<< std::endl;
                return;
        }
        throw std::runtime_error(std::format("Cannot add a table named {}: That name is already taken.",tableName));
}




auto Db::processInsert(std::string const& tableName,std::map<std::string, std::string> const& values)-> void {
    auto const& getTable = this->tableExists(tableName);
    if (getTable != tables.end()) {
        (*getTable)->insertValues(values);
        return;
    }
    throw std::runtime_error(std::format("No table with the name {} was found.",tableName));
}
auto Db::processUpdate(std::string const& tableName, std::map<std::string,std::string> const& values, std::vector<std::string> const& conditions )-> void {
    auto const& getTable = this->tableExists(tableName);
    if (getTable != tables.end()) {
        (*getTable)->updateValues(values,conditions);
        return;
    }
    throw std::runtime_error(std::format("No table with the name {} was found.",tableName));

}
 auto Db::processDelete(std::string const& tableName)-> void {
            auto const& it = this->tableExists(tableName);
            if (it != this->tables.end()) {
                (*it)->clearRows();
                return;
            }
            throw std::runtime_error(std::format("No table with the name {} was found.",tableName));
}
auto Db::processSelect(std::string const& tableName,std::vector<std::string> const& columns)-> void{
        auto const& getTable = this->tableExists(tableName);
        if (getTable != this->tables.end()) {
            (*getTable)->selectColumns(columns);
            return;
        }
        throw std::runtime_error(std::format("No table with the name {} was found.",tableName));
}
auto Db::processSelect(std::string const& tableName,std::vector<std::string> const& columns,std::vector<std::string> const& conditions)-> void {
    auto const& getTable = this->tableExists(tableName);
    if (getTable != this->tables.end()) {
        (*getTable)->selectFilteredColumns(columns,conditions);
        return;
    }
    throw std::runtime_error(std::format("No table with the name {} was found.",tableName));
}
auto Db::processOrderSelect(std::string const& tableName,std::vector<std::string> const& pickedColumns,std::string const& byColumnName)-> void {
    auto const& getTable = this->tableExists(tableName);
    if (getTable != this->tables.end()) {
        (*getTable)->selectOrderedColumns(pickedColumns,byColumnName);
        return;
    }
    throw std::runtime_error(std::format("No table with the name {} was found.",tableName));
}


auto Db::processAlterRename(std::string const& tableName,std::string const& columnName, std::string const& newColumnName)-> void{
    auto const& getTable = this->tableExists(tableName);
    if (getTable != this->tables.end()) {
        (*getTable)->renameColumn(columnName,newColumnName);
        return;
    }
    throw std::runtime_error(std::format("No table with the name {} was found.",tableName));
}
auto Db::processAlterAdd(std::string const& tableName,std::string const& columnName,std::string const& dataType)-> void{
    auto const& getTable = this->tableExists(tableName);
    if (getTable != this->tables.end()) {
        (*getTable)->addColumn(columnName,dataType);
        return;
    }
    throw std::runtime_error(std::format("No table with the name {} was found.",tableName));
}
auto Db::processAlterDelete(std::string const& tableName,std::string const& columnName) -> void {
    auto const& getTable = this->tableExists(tableName);
    if (getTable != this->tables.end()) {
        (*getTable)->dropColumn(columnName);
        return;
    }
    throw std::runtime_error(std::format("No table with the name {} was found.",tableName));
}
auto Db::processTableDrop(std::string const& tableName)-> void {
        auto const& getTable = this->tableExists(tableName);
        if (getTable != this->tables.end()) {
            delete *getTable;
            this->tables.erase(getTable);
            std::cout << "Our table named " << tableName <<" has been successfully deleted." << std::endl;
            return;
        }
        throw std::runtime_error(std::format("No table with the name {} was found.",tableName));
    }




