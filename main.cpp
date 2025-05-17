#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <algorithm>
#include "DB.h"
#include "InterpreterQuery.h"
#include "GalaxyQueryExporter.h"

auto main()-> int {
    Db* db = nullptr;
    std::string const logo = R"(
  ____       _            _  ___
 / ___| __ _| | __ ___  _(_)/ _ \ _   _  ___ _ __ _   _
| |  _ / _` | |/ _` \ \/ / | | | | | | |/ _ \ '__| | | |
| |_| | (_| | | (_| |>  <| | |_| | |_| |  __/ |  | |_| |
 \____|\__,_|_|\__,_/_/\_\_|\__\_\\__,_|\___|_|   \__, |
                                                  |___/
 )";
    std::cout << logo << std::endl;
    db=GalaxyQueryExporter::loadDb();


    auto getQuery = std::string();
    std::cout << "Start typing" << std::endl;


    while (std::getline(std::cin,getQuery) &&  getQuery != "end") {
            try {
                InterpreterQuery::processQuery(db,getQuery);
            }  catch (const std::exception & ex) {
                std::cout <<ex.what() << std::endl;
            }
            std::cout << "Start typing" << std::endl;

    }
    GalaxyQueryExporter::saveToFile(db);





}