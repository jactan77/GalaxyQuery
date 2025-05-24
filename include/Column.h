
#pragma once

#include <map>
#include <string>
#include <ranges>
#include <format>
#include <utility>
#include <vector>
#include <set>
#include <algorithm>
#include <ranges>
#include <iostream>
#include <sstream>
class Column {
      std::string name;
      std::string dataType;
      std::map<int,std::string> fieldValues; // key <=> id

      static auto toInt(std::string const& value)->int;
      auto printHeader(size_t width)->std::string;
      static auto printRow(std::string const& value,size_t width) -> std::string;
      static inline auto defaultValues = std::map<std::string,std::string>{
            {"INT","0"},
            {"STRING","EMPTY"},
            {"CHAR","N"},
            {"BOOL","TRUE"}
      };

      template<typename Comparator>
      [[nodiscard]] auto FilterByValue(std::string const& value, Comparator compare) const -> std::vector<int> {
            if (this->dataType != selectType(value)) {
                  throw std::runtime_error("You are attempting to compare columns with incompatible data types");
            }
            auto filteredIds = std::vector<int>();
            for (auto const& [id,rowValue] : this->fieldValues) {
                  if (this->dataType == "INT" || this->dataType == "BOOL") {
                        if (compare(toInt(rowValue),toInt(value))) {
                              filteredIds.push_back(id);
                        }
                  }else {
                        if (compare(value,rowValue)){
                              filteredIds.push_back(id);
                        }
                  }
            }
            return  filteredIds;
      };

      auto operator==(const std::string& value) const -> std::vector<int> {
            return FilterByValue(value,[](const auto& val1, const auto& val2)->bool{return val1 == val2;});
      }

      auto operator>(const std::string& value) const -> std::vector<int> {
            return FilterByValue(value,[](const auto& val1, const auto& val2)->bool{return val1 > val2;});
      }

      auto operator<(const std::string& value) const -> std::vector<int> {
            return FilterByValue(value,[](const auto& val1, const auto& val2)->bool{return val1 < val2;});
      }

      auto operator>=(const std::string& value) const -> std::vector<int> {
            return FilterByValue(value,[](const auto& val1, const auto& val2)->bool{return val1 >= val2;});
      }

      auto operator<=(const std::string& value) const -> std::vector<int> {
            return FilterByValue(value,[](const auto& val1, const auto& val2)->bool{return val1 <= val2;});
      }
      auto operator!=(const std::string& value) const -> std::vector<int> {
            return FilterByValue(value,[](const auto& val1, const auto& val2)->bool{return val1 != val2;});
      }

public:
      Column(std::string  name, std::string dataType):name(std::move(name)),dataType(std::move(dataType)) {};
      ~Column() = default;
      [[nodiscard]] auto findValue(std::string const& value) const;
      [[nodiscard]] auto getFilteredRows(std::string const& value,std::string const& operand) const-> std::vector<int>;
      [[nodiscard]] auto getName() const -> std::string;
      [[nodiscard]] auto getDataType() const -> std::string {return dataType;}
      [[nodiscard]] auto getRows() const ->std::map<int,std::string>{return fieldValues;}
      auto setFieldValues(const std::map<int,std::string> &newFieldValues)->void;
      auto insertValue(int const &id, std::string const &value)->void;
      auto insertDefaultValues(int ids)->void;
      auto setName(std::string const& newName)->void;
      auto eraseFieldValues()->void;
      auto printRows(const std::set<int>& ids)->std::string;
      auto updateValue(int const& id, std::string const& newValue)->void;
      static auto selectType(std::string const& value )->std::string;
      [[nodiscard]] auto calculateWidth() const -> size_t;

};


