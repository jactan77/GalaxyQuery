

#include "InterpreterQuery.h"
     auto InterpreterQuery::processQuery(std::string input) -> Db{
            auto tokens = getTokens(input);
            auto it = std::ranges::find(GalaxyKeywords.begin(),GalaxyKeywords.end(),tokens.front());

            if (it == GalaxyKeywords.end()) {
                throw;
            }

         if ( "DATABASE" == tokens.at(1)) {
             if (tokens.size() == 3) {
                 return DDL_FUNC::createDB(std::string(tokens.at(2)));
             }
         }
            return DDL_FUNC::createDB(std::string("null"));


     }
    auto InterpreterQuery::getTokens(std::string element)->std::vector<std::string>{
            std::ranges::transform(element,element.begin(),::toupper);
            auto tokens = std::vector<std::string>();
            for (const auto& subrange : std::ranges::split_view(element, ' ')) {
                 std::string_view const sv(subrange.begin(),subrange.end());
                 tokens.push_back(std::string(sv));
            }
            for (auto const& token : tokens ) {
                std::cout << token << std::endl;
            }
         return tokens;
      }