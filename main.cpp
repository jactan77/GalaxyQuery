#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <algorithm>
#include "DataEntry/DB.h"
#include "QueryEngine/InterpreterQuery.h"

auto main()-> int {
    auto session = std::vector<Db>();
    std::string const logo = R"(
  ____       _            _  ___
 / ___| __ _| | __ ___  _(_)/ _ \ _   _  ___ _ __ _   _
| |  _ / _` | |/ _` \ \/ / | | | | | | |/ _ \ '__| | | |
| |_| | (_| | | (_| |>  <| | |_| | |_| |  __/ |  | |_| |
 \____|\__,_|_|\__,_/_/\_\_|\__\_\\__,_|\___|_|   \__, |
                                                  |___/
 )";
    std::cout << logo << std::endl;
    std::cout << "Start typing" << std::endl;

    auto getQuery = std::string();

    std::getline(std::cin,getQuery);
    try {
        InterpreterQuery::processQuery(getQuery);
    }catch (...) {
        std::cout << "WrongQuery" << std::endl;
    }



}