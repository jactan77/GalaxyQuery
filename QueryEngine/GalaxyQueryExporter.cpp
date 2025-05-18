#include "../include/GalaxyQueryExporter.h"




auto GalaxyQueryExporter::saveToFile(Db*& db) -> void {
    std::fstream file("../data.txt",std::ios::out | std::ios::trunc);
    std::stringstream output;
    if (db == nullptr) {
        file << output.str();
        return;
    }
    auto const& dbName = db->getDbName();
    auto const& getTables = db->getTables();
    output << std::format("DB:{}",dbName) << '\n';
    if (!getTables.empty()) {
        for (const Table* table: getTables) {
            auto const& getColumns = table->getColumns();
            output << table->getTableName() << ":";
            for (Column const* column: getColumns) {
                output << column->getName() << std::format("({})",column->getDataType());
                auto rows = column->getRows();
                std::stringstream values;
                for (int i = 1; i <= rows.size(); i++) {
                    if (i == rows.size()) {
                        values << rows[i];
                        continue;
                    }
                    values << rows[i] << ",";
                }
                output << "{" << values.str() << "}";
                if (getColumns.back() == column) {
                    break;
                }
                output << ":";
            }
            output << "\n";
        }

}
    file << output.str();
}
