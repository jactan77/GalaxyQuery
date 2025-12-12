#include "GalaxyQueryExporter.h"

auto GalaxyQueryExporter::saveToFile(std::nullptr_t) -> void {
  std::fstream file("../data.txt", std::ios::out | std::ios::trunc);
  file << "";
}

auto GalaxyQueryExporter::saveToFile(const Db &db) -> void {
  std::fstream file("../data.txt", std::ios::out | std::ios::trunc);
  std::stringstream output;

  output << std::format("DB:{}", db.getDbName()) << '\n';

  for (const auto &table : db.getTables()) {
    output << table->getTableName() << ":";
    const auto &columns = table->getColumns();

    for (size_t colIdx = 0; colIdx < columns.size(); ++colIdx) {
      const auto &column = columns[colIdx];
      output << column->getName() << std::format("({})", column->getDataType());

      auto rows = column->getRows();
      std::stringstream values;
      size_t rowCount = rows.size();
      size_t current = 0;

      for (const auto &[id, value] : rows) {
        values << value;
        if (++current < rowCount) {
          values << ",";
        }
      }

      output << "{" << values.str() << "}";
      if (colIdx < columns.size() - 1) {
        output << ":";
      }
    }
    output << "\n";
  }

  file << output.str();
}
