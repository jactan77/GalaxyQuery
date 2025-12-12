#include "InterpreterQuery.h"

auto InterpreterQuery::processQuery(std::unique_ptr<Db>& db, std::string const& input) -> void {
    auto tokens = getTokens(input);
    
    if (tokens.empty()) {
        throw std::runtime_error("Empty query");
    }

    const auto& command = tokens.at(0);

    if (command == "CREATE" && tokens.size() >= 3 && tokens.at(1) == "DATABASE") {
        if (db) {
            throw std::runtime_error("Cannot create a new database: one already exists");
        }
        db = std::make_unique<Db>(tokens.at(2));
        std::cout << "You have created a database named: " << db->getDbName() << std::endl;
        return;
    }

    if (command == "DROP") {
        handleDrop(db, std::vector(tokens.begin() + 1, tokens.end()));
        return;
    }

    if (!db) {
        throw std::runtime_error("No database exists. Create one first.");
    }

    auto it = queryHandlers.find(command);
    if (it != queryHandlers.end()) {
        it->second(*db, std::vector(tokens.begin() + 1, tokens.end()));
    } else {
        throw std::runtime_error("Unknown command: " + command);
    }
}

auto InterpreterQuery::getTokens(std::string element) -> std::vector<std::string> {
    std::vector<std::string> tokens;
    const std::regex pattern(R"(\(([^)]*)\))");
    auto query = std::regex_replace(element, pattern, " ");

    for (const auto& subrange : std::ranges::split_view(query, ' ')) {
        std::string_view sv(subrange.begin(), subrange.end());
        if (!sv.empty()) {
            tokens.emplace_back(sv);
        }
    }

    for (auto it = std::sregex_iterator(element.begin(), element.end(), pattern); 
         it != std::sregex_iterator(); ++it) {
        tokens.push_back((*it)[1]);
    }
    return tokens;
}

auto InterpreterQuery::handleSelect(Db& db, const std::vector<std::string>& query) -> void {
    if (query.size() < 3 || query.at(0) != "FROM") {
        throw std::runtime_error("Invalid SELECT syntax");
    }

    const auto& tableName = query.at(1);
    
    if (query.size() == 5 && query.at(2) == "WHERE") {
        db.processSelect(tableName, tokenizeColumns(query.at(3)), tokenizeConditions(query.at(4)));
    } else if (query.size() == 5 && query.at(2) == "ORDER_BY") {
        db.processOrderSelect(tableName, tokenizeColumns(query.at(4)), query.at(3));
    } else if (query.size() == 3) {
        db.processSelect(tableName, tokenizeColumns(query.at(2)));
    } else {
        throw std::runtime_error("Invalid SELECT syntax");
    }
}

auto InterpreterQuery::handleInsert(Db& db, const std::vector<std::string>& query) -> void {
    if (query.size() != 5 || query.at(0) != "INTO" || query.at(2) != "VALUES") {
        throw std::runtime_error("Invalid INSERT syntax");
    }

    const auto& tableName = query.at(1);
    auto columns = tokenizeColumns(query.at(3));
    auto parseValues = query.at(4) | std::views::split(',');
    
    std::vector<std::string> values;
    for (auto const& segment : parseValues) {
        std::string word(segment.begin(), segment.end());
        values.push_back(std::regex_replace(word, std::regex("\\s+"), ""));
    }

    if (columns.size() != values.size()) {
        throw std::runtime_error("Column count doesn't match value count");
    }

    std::map<std::string, std::string> colAndVal;
    for (size_t i = 0; i < columns.size(); ++i) {
        colAndVal[columns[i]] = values[i];
    }

    db.processInsert(tableName, colAndVal);
    std::cout << "Data inserted successfully." << std::endl;
}

auto InterpreterQuery::handleUpdate(Db& db, const std::vector<std::string>& query) -> void {
    if (query.size() != 5 || query.at(1) != "SET" || query.at(2) != "WHERE") {
        throw std::runtime_error("Invalid UPDATE syntax");
    }

    const auto& tableName = query.at(0);
    const std::regex pattern(R"((\w+)\s*=\s*(\w+))");
    std::map<std::string, std::string> setValues;

    for (auto const& segment : query.at(3) | std::views::split(',')) {
        std::string val(segment.begin(), segment.end());
        std::smatch matches;
        if (std::regex_search(val, matches, pattern)) {
            setValues[matches[1]] = matches[2];
        }
    }

    db.processUpdate(tableName, setValues, tokenizeConditions(query.at(4)));
}

auto InterpreterQuery::handleCreate(Db& db, const std::vector<std::string>& query) -> void {
    if (query.size() != 3 || query.at(0) != "TABLE" || query.back().empty()) {
        throw std::runtime_error("Invalid CREATE TABLE syntax");
    }

    const auto& tableName = query.at(1);
    const std::regex pattern(R"((\w+)\s+(\w+))");
    std::vector<std::pair<std::string, std::string>> columns;

    for (auto const& segment : query.at(2) | std::views::split(',')) {
        std::string column(segment.begin(), segment.end());
        std::smatch matches;
        if (!std::regex_search(column, matches, pattern)) {
            throw std::runtime_error("Invalid column definition");
        }
        
        std::string datatype = matches[2];
        if (!isValidDataType(datatype)) {
            throw std::runtime_error("Invalid data type: " + datatype);
        }
        columns.emplace_back(matches[1], datatype);
    }

    db.processCreateTable(tableName, columns);
}

auto InterpreterQuery::handleAlter(Db& db, const std::vector<std::string>& query) -> void {
    if (query.size() < 5 || query.at(0) != "TABLE") {
        throw std::runtime_error("Invalid ALTER syntax");
    }

    const auto& tableName = query.at(1);
    const auto& action = query.at(2);

    if (action == "ADD" && query.size() == 5) {
        const auto& dataType = query.at(4);
        if (!isValidDataType(dataType)) {
            throw std::runtime_error("Invalid data type: " + dataType);
        }
        db.processAlterAdd(tableName, query.at(3), dataType);
        std::cout << std::format("Column {} added successfully.", query.at(3)) << std::endl;
    } 
    else if (action == "RENAME" && query.size() == 7 && query.at(3) == "COLUMN") {
        db.processAlterRename(tableName, query.at(4), query.at(6));
        std::cout << std::format("Column renamed to {}.", query.at(6)) << std::endl;
    }
    else if (action == "DROP" && query.size() == 5 && query.at(3) == "COLUMN") {
        db.processAlterDelete(tableName, query.at(4));
        std::cout << std::format("Column {} deleted.", query.at(4)) << std::endl;
    }
    else {
        throw std::runtime_error("Invalid ALTER syntax");
    }
}

auto InterpreterQuery::handleDelete(Db& db, const std::vector<std::string>& query) -> void {
    if (query.size() != 2 || query.at(0) != "FROM") {
        throw std::runtime_error("Invalid DELETE syntax");
    }
    db.processDelete(query.at(1));
}

auto InterpreterQuery::handleDrop(std::unique_ptr<Db>& db, const std::vector<std::string>& query) -> void {
    if (query.size() != 2) {
        throw std::runtime_error("Invalid DROP syntax");
    }

    if (query.at(0) == "DATABASE") {
        db.reset();
        std::cout << "Database deleted." << std::endl;
    } 
    else if (query.at(0) == "TABLE" && db) {
        db->processTableDrop(query.at(1));
    }
    else {
        throw std::runtime_error("Invalid DROP syntax");
    }
}

auto InterpreterQuery::tokenizeConditions(std::string const& parseCondition) -> std::vector<std::string> {
    std::vector<std::string> conditions;
    const std::regex pattern(R"((\w+)\s*([\=\>\<\!=]\=?)\s*(\w+)(?:\s+(AND|OR)\s+)?)");

    for (auto it = std::sregex_iterator(parseCondition.begin(), parseCondition.end(), pattern);
         it != std::sregex_iterator(); ++it) {
        for (int i = 1; i <= 4; ++i) {
            if ((*it)[i].matched) {
                conditions.push_back((*it)[i].str());
            }
        }
    }
    return conditions;
}

auto InterpreterQuery::tokenizeColumns(std::string const& parseColumns) -> std::vector<std::string> {
    std::vector<std::string> columns;
    for (auto const& segment : parseColumns | std::views::split(',')) {
        std::string word(segment.begin(), segment.end());
        columns.push_back(std::regex_replace(word, std::regex("\\s+"), ""));
    }
    return columns;
}
