

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



auto Db::processCreate(std::vector<std::string> const& createQuery)-> void {
    if (createQuery.at(0) == "TABLE") {
        if (createQuery.size() != 3) {
        throw;
      }
        const std::regex columnPattern(R"((\w+)\s+(\w+))");
        std::smatch matches;
        auto parseColumns = createQuery.at(2) | std::views::split(',');
        auto getColumns = std::map<int,std::pair<std::string,std::string>>();
        int index = 0;
        for (auto const& segment: parseColumns) {
            std::string column(segment.begin(), segment.end());
            if (std::regex_search(column, matches, columnPattern)) {

                auto const name = std::string(matches[1]);
                auto const type = std::string(matches[2]);

              if (std::ranges::find(dataTypes, type) == dataTypes.end()) {
                    std::cout << "Error: Invalid data type '" << type << "'" << std::endl;
                    throw std::runtime_error("Invalid data type: " + type);
              }

              std::cout <<"name: " << name << std::endl;
              std::cout << "type: " << type << std::endl;
                std::pair<std::string, std::string> pair(name, type);
                getColumns.insert({++index,pair});
            }
        }

        this->setTable(new Table(createQuery.at(1),getColumns));
        std::cout << "You have created in DB: " << this->name << " a table named : " << createQuery.at(1)<< std::endl;
        return;
  }
    throw std::runtime_error("");
}


auto Db::processInsert(std::vector<std::string> const& insertQuery)-> void {
        if (insertQuery.size() == 3 && insertQuery.at(0) == "INTO") {
            auto const& getName = insertQuery.at(1);
            auto const it = this->tableExists(getName);
            if (it != this->tables.end()) {
                auto row = std::vector<std::string>();
                auto parseData = insertQuery.at(2) | std::views::split(',');
                auto* table = *it;
                auto const& columns = (table)->columns;
                for (auto const& segment: parseData) {
                    auto const data = std::regex_replace(std::string(segment.begin(),segment.end()),std::regex("\\s+"),"");
                    std::cout << data << std::endl;
                    row.push_back(data);
                }
                if (columns.size() != row.size()) {
                    throw std::runtime_error("Missing values for required columns.");
                }
                int index = 0;
                for (auto const& data: row) {
                    std::pair<std::string,std::string> const& pairColumn = columns.at(++index);
                    if (getDataType(data) != pairColumn.second) {
                        throw std::runtime_error(std::format("The provided value {} is incompatible with the data type of the column {}.",data,pairColumn.first));
                    }
                }
                srand(time(nullptr));
                int id = (rand()%1000) + 1;
                table->rows.insert({id,row});
                std::cout << "The data has been successfully inserted." << std::endl;
            }

        }
}
auto Db::processUpdate(std::vector<std::string> const& updateQuery)-> void {

}
 auto Db::processDelete(std::vector<std::string> const& deleteQuery)-> void {
        if (deleteQuery.at(0) == "FROM" && deleteQuery.size() == 2 ) {
            auto const& getName = deleteQuery.at(1);
            auto const it = this->tableExists(getName);
            if (it != this->tables.end()) {
                (*it)->rows.clear();
                std::cout << std::format("All rows from the table {} have been deleted.", (*it)->name) << std::endl;
                return;
            }
            throw std::runtime_error(std::format("No table with the name {} was found.",getName));
        }
        throw std::runtime_error("Invalid operation");

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
auto Db::processAlter(std::vector<std::string> const& alterQuery)-> void{
    if ((alterQuery.size() == 5 || alterQuery.size() == 7) && alterQuery.at(0) == "TABLE") {
        auto const& getName = alterQuery.at(1);
        auto it = this->tableExists(getName);
        if (it != this->tables.end()) {
            auto* getTable = *it;
            if (alterQuery.at(2) == "ADD") {
                auto const& columnName = alterQuery.at(3);
                auto const& dataType = alterQuery.at(4);
                getTable->addColumn(columnName,dataType);
                std::cout << std::format("The column has been successfully added with the name {}.",columnName)<< std::endl;
                return;
            }
            if (alterQuery.at(2) == "RENAME" && alterQuery.at(3) == "COLUMN") {
                auto const& oldName = alterQuery.at(4);
                auto const& newName = alterQuery.at(6);
                getTable->renameColumn(oldName,newName);
                std::cout << std::format("The column has been successfully renamed to {}.",newName) << std::endl;
                return;
            }
        }
        throw std::runtime_error(std::format("No table with the name {} was found.",getName));
    }
}
auto Db::processDrop(std::vector<std::string> const& dropQuery)-> void {
    if (dropQuery.at(0) == "TABLE" && dropQuery.size() == 2) {
        auto const& getName = dropQuery.at(1);
        auto const& it = this->tableExists(getName);
        if (it != this->tables.end()) {
            delete *it;
            this->tables.erase(it);
            std::cout << "Our table named " << getName <<" has been successfully deleted." << std::endl;
            return;
        }
        throw std::runtime_error(std::format("No table with the name {} was found.",getName));
    }

}

auto Db::getDataType(std::string const& value)->std::string {
    auto isInt = [](std::string const& str)-> bool {
        const auto isNegative = str.at(0) == '-' ? 1 : 0;
        return std::ranges::all_of(str.begin()+isNegative,str.end(),::isdigit);
    };
    if (isInt(value)) {
        return "INT";
    }
    if (value == "TRUE" or value == "FALSE") {
        return "BOOL";
    }
    if (value.size() == 1) {
        return "CHAR";
    }
    return "STRING";

}
