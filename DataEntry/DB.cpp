

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



auto Db::processCreateTable(std::string const& tableName, std::map<std::string,std::string> const& columns)-> void {
        this->setTable(new Table(tableName,columns));
        std::cout << "You have created in DB: " << this->name << " a table named : " << tableName<< std::endl;
}




auto Db::processInsert(std::string const& tableName,std::map<std::string, std::string> const& values)-> void {
    auto const& getTable = this->tableExists(tableName);
    if (getTable != tables.end()) {
        (*getTable)->insertValues(values);
        std::cout << std::format("INSERT operation completed. Record ID: {}.",(*getTable)->id);
        return;
    }
    throw std::runtime_error(std::format("No table with the name {} was found.",tableName));
}
auto Db::processUpdate(std::vector<std::string> const& updateQuery)-> void {
        if (updateQuery.size() > 1) {
            auto const& getName = updateQuery.at(0);
            auto const it = this->tableExists(getName);
            if (it != this->tables.end() && updateQuery.at(1) == "SET") {
                const std::regex getColumnPatter(R"((\w+)\s*=\s*(\w+))");
                std::smatch matches;
                auto* table = *it;
                auto columnValues = std::vector<std::pair<std::string,std::string>>();
                auto columnUpdates = updateQuery.back() | std::views::split(',');
                for (auto const& segment: columnUpdates) {
                    std::string columnValue(segment.begin(),segment.end());
                    if (std::regex_search(columnValue,matches,getColumnPatter)) {
                            auto const& columnName = matches[1];
                            auto const& value = matches[2];
                            std::cout << columnName << std::endl;
                            std::cout << value << std::endl;



                    }
                }

            }
        }
}
 auto Db::processDelete(std::string const& tableName)-> void {
            auto const& it = this->tableExists(tableName);
            if (it != this->tables.end()) {
                (*it)->clearRows();
                return;
            }
            throw std::runtime_error(std::format("No table with the name {} was found.",tableName));
}
auto Db::processSelect(std::vector<std::string> const& selectQuery)-> void{
                        /* ex
                    ---------------------------------------
                    | ID         | Name        | Age      |
                    |------------|-------------|----------|
                    | 1          | Alice       | 25       |
                    | 2          | Bob         | 30       |
                    | 3          | Charlie     | 35       |
                    |------------|-------------|----------|
                        */
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




