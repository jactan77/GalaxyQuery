

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
    if ((query.size() == 3 || query.size() == 5) && query.at(0) == "FROM") {
            auto const& tableName = query.at(1);
            if (query.size() == 5 && query.at(2) == "WHERE") {
                auto const& getColumns = tokenizeColumns(query.at(3));
                auto const& getConditions = tokenizeConditions(query.at(4));
                db->processSelect(tableName,getColumns,getConditions);
                return;
            }
            auto const& getColumns  = tokenizeColumns(query.at(2));
            db->processSelect(tableName,getColumns);
            return;

    }
    throw std::runtime_error("Invalid operation");
}
auto InterpreterQuery::tokenizeInsertQuery 	(Db*& db, const std::vector<std::string>& query) -> void {
    if (query.size() == 5 && query.at(0) == "INTO" && query.at(2) == "VALUES") {

            auto const& tableName = query.at(1);
            auto parseValues = query.at(4) |  std::views::split(',');

            auto const& columns = tokenizeColumns(query.at(3));
            auto values = std::vector<std::string>(std::ranges::distance(parseValues));
            auto colAndVal = std::map<std::string,std::string>(); // column and value


            std::ranges::transform(parseValues,values.begin(),[](auto const& segment)->std::string{
                std::string word(segment.begin(),segment.end());
                word = std::regex_replace(word,std::regex("\\s+"),"");
                std::cout << std::format("Value {}", word)<< std::endl;
                return word;
            });
            if (columns.size() != values.size()) {
                throw std::runtime_error("The number of columns does not match the number of values.");
            }
            for (int i = 0; i < columns.size(); i++) {
                colAndVal[columns[i]] = values[i];
            }
            db->processInsert(tableName,colAndVal);
            std::cout << "The data has been successfully inserted." << std::endl;
            return;

    }
            throw std::runtime_error("Invalid operation");
}
auto InterpreterQuery::tokenizeUpdateQuery(Db*& db, const std::vector<std::string>& query) -> void {
    if (query.size() == 5 && query.at(1) == "SET" && query.at(2) == "WHERE") {
        auto const& tableName = query.at(0);
        auto parseSetValues = query.at(3) | std::views::split(',');
        auto setValues =std::map<std::string,std::string>();
        const std::regex columnValuePattern(R"((\w+)\s*=\s*(\w+))");
        std::smatch matches;
        for (auto const& segment:  parseSetValues) {
            std::string val(segment.begin(),segment.end());
            if (std::regex_search(val,matches,columnValuePattern)) {
                std::string const& columnName = matches[1];
                std::string const& value = matches[2];
                setValues.insert({columnName,value});
            }
        }

        const auto getConditions = tokenizeConditions(query.at(4));

        db->processUpdate(tableName,setValues,getConditions);
        return;
    }
    throw std::runtime_error("Invalid operation");

}
auto InterpreterQuery::tokenizeConditions(std::string const& parseCondition) -> std::vector<std::string> {
    auto getCondition = std::vector<std::string>();
    const std::regex conditionPattern(R"((\w+)\s*([\=\>\<\!=]\=?)\s*(\w+)(?:\s+(AND|OR)\s+)?)");
    const auto it_begin=std::sregex_iterator(parseCondition.begin(),parseCondition.end(),conditionPattern);
    const auto it_end = std::sregex_iterator();
    for (auto it=it_begin; it != it_end; ++it) {
        std::smatch const& match = *it;
        for (int i = 1; i <= 4; i++) {
            if (match[i].matched) {
                getCondition.push_back(match[i].str());
            }
        }
    }
    for (auto const& ww: getCondition) {
        std::cout << ww << std::endl;
    }
    return getCondition;
}
auto InterpreterQuery::tokenizeColumns(std::string const& parseColumns)-> std::vector<std::string> {
    auto getColumns = parseColumns | std::views::split(',');
    auto columns = std::vector<std::string>(std::ranges::distance(getColumns));
    std::ranges::transform(getColumns,columns.begin(),[](auto const& segment)->std::string {
                std::string word(segment.begin(),segment.end());
                word = std::regex_replace(word,std::regex("\\s+"),"");
                std::cout << std::format("column:{}", word)<< std::endl;
                return word;
    });
    return columns;
}

auto InterpreterQuery::tokenizeCreateQuery(Db*& db, const std::vector<std::string>& query) -> void {
    if (query.at(0) == "TABLE") {
        if (query.size() != 3 || query.back().empty() || query.at(2).find(',') == std::string::npos) {
            throw std::runtime_error("Invalid operation");
        }
        const std::regex columnPattern(R"((\w+)\s+(\w+))");
        std::smatch matches;
        auto const& tableName = query.at(1);
        auto parseColumns = query.at(2) | std::views::split(',');
        auto getColumns = std::vector<std::pair<std::string,std::string>>();
        for (auto const& segment: parseColumns) {
            std::string column(segment.begin(), segment.end());
            if (std::regex_search(column, matches, columnPattern)) {

                auto const name = std::string(matches[1]);
                auto const datatype = std::string(matches[2]);

                if (std::ranges::find(dataTypes, datatype) == dataTypes.end()) {
                    std::cout << "Error: Invalid data type '" << datatype << "'" << std::endl;
                    throw std::runtime_error("Invalid data type: " + datatype);
                }

                std::cout <<"name: " << name << std::endl;
                std::cout << "type: " << datatype << std::endl;
                getColumns.emplace_back(name,datatype);
            } else {
                throw std::runtime_error("Invalid operation");
            }
        }
        db->processCreateTable(tableName,getColumns);
        return;
    }
    throw std::runtime_error("Invalid operation");
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
            if (std::ranges::find(dataTypes, dataType) == dataTypes.end()) {
                std::cout << "Error: Invalid data type '" << dataType << "'" << std::endl;
                throw std::runtime_error("Invalid data type: " + dataType);
            }
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
       if (query.at(2)=="DROP" && query.at(3)=="COLUMN") {
           auto const& columnName = query.at(4);
           db->processAlterDelete(tableName,columnName);
           std::cout << std::format("The column {} has been successfully deleted.",columnName) << std::endl;
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
