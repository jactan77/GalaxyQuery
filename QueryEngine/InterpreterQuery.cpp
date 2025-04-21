

#include "InterpreterQuery.h"



auto InterpreterQuery::processQuery(std::string input) -> void{
    auto tokens = getTokens(input); // tokens.front must match one of {"SELECT", "INSERT", "UPDATE", "DELETE", "CREATE", "ALTER", "DROP"}
            auto it = std::ranges::find(GalaxyKeywords.begin(),GalaxyKeywords.end(),tokens.front());
            if (it == GalaxyKeywords.end()) {
                throw;
            }





}

auto InterpreterQuery::getTokens(std::string element)->std::vector<std::string>{
            std::ranges::transform(element,element.begin(),::toupper);
            auto tokens = std::vector<std::string>();
            std::cout << std::ranges::count_if(element,[](char const& x)-> bool {
                if (x == '(' || x == ')')
                        return true;
                return false;
            });
            for (const auto& subrange : std::ranges::split_view(element, ' ')) {
                 std::string_view const sv(subrange.begin(),subrange.end());
                 tokens.push_back(std::string(sv));
            }
            for (auto const& token : tokens ) {
                std::cout << token << std::endl;
            }
         return tokens;
}