
#include "QueryHandler.h"

#include <regex>


auto QueryHandler::processCreate(std::vector<std::string> createQuery)-> void {
  if (createQuery.size() != 2 ) {
    throw;
  }
  if (createQuery.at(0) == "DATABASE") {
    std::cout << "You have created a database named : " << Db(std::string(createQuery.at(1))).name;
  }
  if (createQuery.at(0) == "TABLE") {
      if (createQuery.size() != 2) {
        throw;
      }
        const std::regex columnPattern(R"((\w+)\s+(\w+))");
        std::smatch matches;
        auto parseColumns = createQuery.at(1) | std::views::split(',');

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
  }
}


auto QueryHandler::processInsert(std::vector<std::string> insertQuery)-> void {
 ///////////////
}
auto QueryHandler::processUpdate(std::vector<std::string> updateQuery)-> void {
  //////////////
}
 auto QueryHandler::processDelete(std::vector<std::string> deleteQuery)-> void {
 ////////////////////
}
auto QueryHandler::processSelect(std::vector<std::string> selectQuery)-> void{
  ///////////
}
auto QueryHandler::processAlter(std::vector<std::string> alterQuery)-> void{
  ///////////
}
auto QueryHandler::processDrop(std::vector<std::string> dropQuery)-> void {
  /////////////
}