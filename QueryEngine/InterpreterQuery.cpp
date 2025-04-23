

#include "InterpreterQuery.h"

#include "../QueryProcess/QueryHandler.h"


auto InterpreterQuery::processQuery(std::string input) -> void{
    auto tokens = getTokens(input); // tokens.front must match one of {"SELECT", "INSERT", "UPDATE", "DELETE", "CREATE", "ALTER", "DROP"}
            auto it = std::ranges::find(GalaxyKeywords.begin(),GalaxyKeywords.end(),tokens.front());
            if (it == GalaxyKeywords.end()) {
                throw std::runtime_error("Wrong Query");
            }
            if (tokens.at(0) == "CREATE") {
                    QueryHandler::processCreate(std::vector(tokens.begin()+1,tokens.end()));
            }

}

auto InterpreterQuery::getTokens(std::string element)->std::vector<std::string>{
            std::ranges::transform(element,element.begin(),::toupper);
            auto tokens = std::vector<std::string>();

            const std::regex pattern(R"(\(([^)]*)\))");
            std::smatch match;
            if ( std::regex_search(element,match,pattern) ) {
                auto query = std::regex_replace(element,pattern," ");
                for (const auto& subrange : std::ranges::split_view(query, ' ')) {
                    std::string_view const sv(subrange.begin(),subrange.end());
                    if (!sv.empty()) {
                        tokens.push_back(std::string(sv));
                    }
                }
                tokens.push_back(match[1]);
            }else {
                for (const auto& subrange : std::ranges::split_view(element, ' ')) {
                    std::string_view const sv(subrange.begin(),subrange.end());
                    if (!sv.empty()) {
                        tokens.push_back(std::string(sv));
                    }
                }
            }
            for (auto const& token : tokens ) {
                std::cout << token << std::endl;
            }
         return tokens;
}