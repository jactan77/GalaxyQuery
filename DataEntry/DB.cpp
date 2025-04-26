

#include "DB.h"

#include <format>


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
auto Db::getName() -> std::string {
    return this->name;
}
auto Db::cleanTables()  ->void {
    for (auto const* table : this->tables) {
            delete table;
    }
    this->tables.clear();
}



auto Db::processCreate(std::vector<std::string> const& createQuery)-> void {
    if (createQuery.at(0) == "TABLE") {
      if (createQuery.size() != 3) {
        throw;
      }
        const std::regex columnPattern(R"((\w+)\s+(\w+))");
        std::smatch matches;
        auto parseColumns = createQuery.at(2) | std::views::split(',');

        auto getColumns = std::map<std::string,std::string>();
        for (auto const& segment: parseColumns) {
            std::string column(segment.begin(), segment.end());
            if (std::regex_search(column, matches, columnPattern)) {
                    auto const name = std::string(matches[1]);
                    auto const type = std::string(matches[2]);

              if (std::ranges::find(dataTypes, type) == dataTypes.end()) {
                    std::cout << "Error: Invalid data type '" << type << "'" << std::endl;
                    throw std::runtime_error("Invalid data type: " + type);
              }

              std::cout << "name: " << name << std::endl;
              std::cout << "type: " << type << std::endl;
              getColumns.insert({name, type});

          }
        }
        this->setTable(new Table(createQuery.at(1),getColumns));
        std::cout << "You have created in DB: " << this->name << " a table named : " << createQuery.at(1)<< std::endl;
        return;
  }
    throw std::runtime_error("");
}


auto Db::processInsert(std::vector<std::string> const& insertQuery)-> void {
 ///////////////
}
auto Db::processUpdate(std::vector<std::string> const& updateQuery)-> void {
  //////////////
}
 auto Db::processDelete(std::vector<std::string> const& deleteQuery)-> void {
        if (deleteQuery.at(0) == "FROM" && deleteQuery.size() == 2 ) {
            auto const& getName = deleteQuery.at(1);
            auto const it = std::ranges::find_if(this->tables,[&](const Table* t)-> bool{
                    return t->name == getName;
                });
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
  ///////////
}
auto Db::processAlter(std::vector<std::string> const& alterQuery)-> void{
  ///////////
}
auto Db::processDrop(std::vector<std::string> const& dropQuery)-> void {
    if (dropQuery.at(0) == "TABLE" && dropQuery.size() == 2) {
        auto const& getName = dropQuery.at(1);
        auto const it = std::ranges::find_if(this->tables,[&](const Table* t)-> bool{
            return t->name == getName;
        });
        if (it != this->tables.end()) {
            delete *it;
            this->tables.erase(it);
            std::cout << "Our table named " << getName <<" has been successfully deleted." << std::endl;
            return;
        }
        throw std::runtime_error(std::format("No table with the name {} was found.",getName));
    }

}

