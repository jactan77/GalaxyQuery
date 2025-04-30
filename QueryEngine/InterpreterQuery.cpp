

#include "InterpreterQuery.h"




auto InterpreterQuery::processQuery(Db*& db,std::string const& input) -> void{
            auto tokens = getTokens(input); // tokens.front must match one of {"SELECT", "INSERT", "UPDATE", "DELETE", "CREATE", "ALTER", "DROP"}
            const auto it = GalaxyKeywords.find(tokens.at(0));
            if (db == nullptr && tokens.at(0) == "CREATE" && tokens.at(1)=="DATABASE" && tokens.size() == 3) {
                db = new Db(std::string(tokens.at(2)));
                std::cout << "You have created a database named : " << db->getDbName() << std::endl;
                return;
            }
            if (it != GalaxyKeywords.end() && db != nullptr) {
                it->second(db,std::vector(tokens.begin()+1,tokens.end()));
            } else {
                throw std::runtime_error("Invalid operation or your database doesn't exist");
            }
}

auto InterpreterQuery::getTokens(std::string element)->std::vector<std::string> {
    std::ranges::transform(element,element.begin(),::toupper);
    auto tokens = std::vector<std::string>();

    const std::regex pattern(R"(\(([^)]*)\))");
    const auto element_begin = std::sregex_iterator(element.begin(),element.end(),pattern);
    const auto element_end = std::sregex_iterator();
    auto query = std::regex_replace(element,pattern," ");
    for (const auto& subrange : std::ranges::split_view(query, ' ')) {
        std::string_view const sv(subrange.begin(),subrange.end());
        if (!sv.empty()) {
            tokens.push_back(std::string(sv));
        }
    }for (auto it = element_begin; it != element_end; ++it) {
        auto& match = *it;
        tokens.push_back(match[1]);
    }
            for (auto const& token : tokens ) {
                std::cout << token << std::endl;
            }
         return tokens;
}
auto InterpreterQuery::tokenizeSelectQuery (Db*& db, const std::vector<std::string>& query) -> void {

}
auto InterpreterQuery::tokenizeInsertQuery 	(Db*& db, const std::vector<std::string>& query) -> void {
    if (query.size() == 5 && query.at(0) == "INTO" && query.at(3) == "VALUES") {

            auto const& tableName = query.at(1);
            auto parseColumns = query.at(2) | std::views::split(',');
            auto parseValues = query.at(4) |  std::views::split(',');

            auto columns = std::vector<std::string>();
            auto values = std::vector<std::string>();
            auto colAndVal = std::map<std::string,std::string>(); // column and value

            std::ranges::transform(parseColumns,columns.begin(),[](auto const& segment){
                return std::regex_replace(std::string(segment.begin(),segment.end()),std::regex("\\s+"),"");
            });
            std::ranges::transform(parseValues,values.begin(),[](auto const& segment){
                return std::regex_replace(std::string(segment.begin(),segment.end()),std::regex("\\s+"),"");
            });
            if (columns.size() != values.size()) {
                throw std::runtime_error("The number of columns does not match the number of values.");
            }
            for (int i = 0; i < columns.size(); i++) {
                colAndVal[columns[i]] = values[i];
            }
            db->processInsert(tableName,colAndVal);
            std::cout << "The data has been successfully inserted." << std::endl;


    }
}
auto InterpreterQuery::tokenizeUpdateQuery(Db*& db, const std::vector<std::string>& query) -> void {

}

auto InterpreterQuery::tokenizeCreateQuery(Db*& db, const std::vector<std::string>& query) -> void {
    if (query.at(0) == "TABLE") {
        if (query.size() != 3) {
            throw;
        }
        const std::regex columnPattern(R"((\w+)\s+(\w+))");
        std::smatch matches;
        auto const& tableName = query.at(1);
        auto parseColumns = query.at(2) | std::views::split(',');
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

                std::cout <<"name: " << name << std::endl;
                std::cout << "type: " << type << std::endl;
                getColumns.insert({name,type});
            }
        }
        db->processCreateTable(tableName,getColumns);
    }
}
auto InterpreterQuery::tokenizeDropQuery (Db*& db, const std::vector<std::string>& query) -> void {
    if (query.at(0)=="DATABASE" && query.size() == 2) {
        if (db != nullptr) {
            db->cleanTables();
            std::cout << "Your database has been deleted." << std::endl;
            delete db;
        }
        db = nullptr;
        return;
    }
    if (query.at(0) == "TABLE" && query.size() == 2 && db != nullptr) {
        auto const& tableName = query.at(1);
        db->processTableDrop(tableName);
    }
}


auto InterpreterQuery::tokenizeAlterQuery (Db*& db, const std::vector<std::string>& query) -> void {
    if ((query.size() == 5 || query.size() == 7) && query.at(0) == "TABLE") {
        auto const& tableName = query.at(1);

        if (query.at(2) == "ADD") {
            auto const& columnName = query.at(3);
            auto const& dataType = query.at(4);
            db->processAlterAdd(tableName,columnName,dataType);
            std::cout << std::format("The column has been successfully added with the name {}.",columnName)<< std::endl;
            return;
        }
        if (query.at(2) == "RENAME" && query.at(3) == "COLUMN") {
            auto const& columnName = query.at(4);
            auto const& newColumnName = query.at(6);
            db->processAlterRename(tableName,columnName,newColumnName);
            std::cout << std::format("The column has been successfully renamed to {}.",newColumnName) << std::endl;
            return;
        }
    }
    throw std::runtime_error("Invalid operation");

}
auto InterpreterQuery::tokenizeDeleteQuery(Db*& db, const std::vector<std::string>& query) -> void {
    if (query.at(0) == "FROM" && query.size() == 2 ) {
        auto const& tableName = query.at(1);
        db->processDelete(tableName);
        return;
    }
    throw std::runtime_error("Invalid operation");
}
