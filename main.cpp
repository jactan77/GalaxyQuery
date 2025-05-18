#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <algorithm>
#include "DB.h"
#include "InterpreterQuery.h"
#include "GalaxyQueryExporter.h"
#include "GalaxyQueryLoader.h"


auto main()-> int {
    Db* db = nullptr;
    std::string const logo = R"(
 $$$$$$\   $$$$$$\  $$\        $$$$$$\  $$\   $$\ $$\     $$\  $$$$$$\  $$\   $$\ $$$$$$$$\ $$$$$$$\ $$\     $$\
$$  __$$\ $$  __$$\ $$ |      $$  __$$\ $$ |  $$ |\$$\   $$  |$$  __$$\ $$ |  $$ |$$  _____|$$  __$$\\$$\   $$  |
$$ /  \__|$$ /  $$ |$$ |      $$ /  $$ |\$$\ $$  | \$$\ $$  / $$ /  $$ |$$ |  $$ |$$ |      $$ |  $$ |\$$\ $$  /
$$ |$$$$\ $$$$$$$$ |$$ |      $$$$$$$$ | \$$$$  /   \$$$$  /  $$ |  $$ |$$ |  $$ |$$$$$\    $$$$$$$  | \$$$$  /
$$ |\_$$ |$$  __$$ |$$ |      $$  __$$ | $$  $$<     \$$  /   $$ |  $$ |$$ |  $$ |$$  __|   $$  __$$<   \$$  /
$$ |  $$ |$$ |  $$ |$$ |      $$ |  $$ |$$  /\$$\     $$ |    $$ $$\$$ |$$ |  $$ |$$ |      $$ |  $$ |   $$ |
\$$$$$$  |$$ |  $$ |$$$$$$$$\ $$ |  $$ |$$ /  $$ |    $$ |    \$$$$$$ / \$$$$$$  |$$$$$$$$\ $$ |  $$ |   $$ |
 \______/ \__|  \__|\________|\__|  \__|\__|  \__|    \__|     \___$$$\  \______/ \________|\__|  \__|   \__|
                                                                   \___|


 )";
    std::cout << logo << std::endl;
    db=GalaxyQueryLoader::loadDb();
    if (db != nullptr) {
        std::cout << std::format("Your database: {} has been loaded.",db->getDbName())<< std::endl;
    }
    auto getQuery = std::string();
    std::cout << "Start typing" << std::endl;


    while (std::getline(std::cin,getQuery) && getQuery != "END" && getQuery != "end") {
        try {
            std::ranges::transform(getQuery,getQuery.begin(),::toupper);
            if (getQuery ==  "SAVE") {
                std::cout << "Saving changes to a file." << std::endl;
                GalaxyQueryExporter::saveToFile(db);
                std::cout << "Start typing" << std::endl;
                continue;
            }
            if (!getQuery.empty()) {
                InterpreterQuery::processQuery(db,getQuery);
            }
        }  catch (const std::exception & ex) {
            std::cout << ex.what() << std::endl;
        }
        std::cout << "Start typing" << std::endl;

    }
}
