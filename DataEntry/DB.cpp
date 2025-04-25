

#include "DB.h"



Db::Db(std::string const& name){
    this->name = name;
    this->tables = std::vector<Table>();
}
Db::Db() {
    this->name = std::string("null");
    this->tables = std::vector<Table>();
}
auto Db::setTable(Table const &t) -> void {
    this->tables.push_back(t);
}
auto Db::getName() -> std::string {
    return this->name;
}



auto Db::processCreate(std::vector<std::string> createQuery)-> void {
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
        this->setTable(Table(createQuery.at(1),getColumns));
        std::cout << "You have created in DB: " << this->name << " a table named : " << createQuery.at(1)<< std::endl;
        return;
  }
    throw std::runtime_error("");
}


auto Db::processInsert(std::vector<std::string> insertQuery)-> void {
 ///////////////
}
auto Db::processUpdate(std::vector<std::string> updateQuery)-> void {
  //////////////
}
 auto Db::processDelete(std::vector<std::string> deleteQuery)-> void {
 ////////////////////
}
auto Db::processSelect(std::vector<std::string> selectQuery)-> void{
  ///////////
}
auto Db::processAlter(std::vector<std::string> alterQuery)-> void{
  ///////////
}
auto Db::processDrop(std::vector<std::string> dropQuery)-> void {
  /////////////
}

