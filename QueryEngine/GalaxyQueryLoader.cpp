#include "GalaxyQueryLoader.h"

auto GalaxyQueryLoader::loadDb()-> Db* {
    std::fstream file("../data.txt",std::ios::in);
    std::string line;
    if (!file || std::filesystem::file_size("../data.txt") == 0 ) {
        return nullptr;
    }
    std::getline(file,line);
    auto const getDbName = parseName(line,std::regex(R"(^DB:(\w+)$)"));
    auto* db = new Db(getDbName);
    while (std::getline(file,line)) {
        if (!line.empty()) {
            parseData(db,line);
        }
    }
    return db;
}
auto GalaxyQueryLoader::parseName(const std::string& line,std::regex const& pattern)-> std::string {
    auto name = std::string();
    std::smatch match;
    if (std::regex_search(line,match,pattern)) {
        name = match[1];
    }
    return name;
}

auto GalaxyQueryLoader::parseData(Db*& db,std::string line)->void {
    auto const& getTableName = parseName(line,std::regex(R"(^(\w+):)"));
    line=std::regex_replace(line,std::regex(R"(^(\w+):)"),"");
    auto parseColumns = std::ranges::views::split(line,':')
                            | std::ranges::views::transform([](auto const& subrange) {
                                    return std::string(subrange.begin(),subrange.end());
                            });
    auto columns = std::vector<Column*>();
    size_t ids = 1;
    for (auto subrange: parseColumns) {
        const auto getColumnName  = parseName(subrange,std::regex(R"(^(\w+)\()"));
        const auto getDataType = parseName(subrange,std::regex(R"(^\w+\((\w+)\))"));
        auto* createColumn = new Column(getColumnName,getDataType);
        auto getFieldValues = parseValues(subrange);
        ids = getFieldValues.size();
        createColumn->setFieldValues(getFieldValues);
        columns.push_back(createColumn);
    }
    if (!columns.empty()) {
        auto* createTable = new Table(getTableName,columns,static_cast<int>(++ids));
        db->setTable(createTable);
    }


}
auto GalaxyQueryLoader::parseValues(const std::string& line)->std::map<int,std::string> {
    auto getValues= parseName(line,std::regex(R"(^\w+\(\w+\)\{([^}]+)\})"));
    auto parseValues = std::ranges::views::split(getValues,',')
                            | std::ranges::views::transform([](auto const& subrange) {
                                    return std::string(subrange.begin(),subrange.end());
                            });
    auto fieldValues = std::map<int,std::string>();
    int id = 1;
    for (auto it = parseValues.begin(); it != parseValues.end(); ++it) {
        fieldValues.insert({id,*it});
        ++id;
    }
    return fieldValues;


}