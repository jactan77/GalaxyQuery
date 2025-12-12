#include "DB.h"
#include "GalaxyQueryExporter.h"
#include "GalaxyQueryLoader.h"
#include "InterpreterQuery.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <ranges>
#include <string>

auto main() -> int {
  std::string const logo = R"(
 $$$$$$\   $$$$$$\  $$\        $$$$$$\  $$\   $$ $$\     $$\  $$$$$$\  $$\   $$\ $$$$$$$$\ $$$$$$$\ $$\     $$\
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

  auto db = GalaxyQueryLoader::loadDb();

  if (db) {
    std::cout << std::format("Your database: {} has been loaded.",
                             db->getDbName())
              << std::endl;
  }

  std::string query;
  std::cout << "Start typing" << std::endl;

  while (std::getline(std::cin, query) && query != "END" && query != "end") {
    try {
      std::ranges::transform(query, query.begin(), ::toupper);

      if (query == "SAVE") {
        std::cout << "Saving changes to a file." << std::endl;
        if (db) {
          GalaxyQueryExporter::saveToFile(*db);
        } else {
          GalaxyQueryExporter::saveToFile(nullptr);
        }
        std::cout << "Start typing" << std::endl;
        continue;
      }

      if (!query.empty()) {
        InterpreterQuery::processQuery(db, query);
      }
    } catch (const std::exception &ex) {
      std::cout << ex.what() << std::endl;
    }
    std::cout << "Start typing" << std::endl;
  }

  return 0;
}
