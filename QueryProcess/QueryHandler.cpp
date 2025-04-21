//
// Created by budix on 4/22/2025.
//

#include "QueryHandler.h"

auto QueryHandler::processCreate(std::vector<std::string> createQuery)-> void{
      if (createQuery.size() != 2 ) {
          throw;
      }
      if (createQuery.at(0) == "DATABASE") {
            std::cout << "You have created a database named : " << Db(std::string(createQuery.at(1))).name;
      }
      if (createQuery.at(0) == "TABLE") {
        //
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