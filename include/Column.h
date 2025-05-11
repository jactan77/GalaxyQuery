
#pragma once

#include <map>
#include <string>
#include <ranges>
#include <format>
#include <utility>
#include <vector>
#include <iostream>
#include <sstream>
class Column {
      std::string name;
      std::string dataType;
      std::map<int,std::string> fieldValues; // key <=> id
      static auto selectType(std::string const& value )->std::string;
      static auto toInt(std::string const& value)->int;
      auto printHeader(size_t width)->std::string;
      static auto printRow(std::string const& value,size_t width) -> std::string;

      auto calculateWidth() const -> size_t;


      template<typename Comparator>
      [[nodiscard]] auto FilterByValue(std::string const& value, Comparator compare) const -> std::vector<int> {
            if (this->dataType != selectType(value)) {
                  throw std::runtime_error("You are attempting to compare columns with incompatible data types");
            }
            auto filteredIds = std::vector<int>();
            for (auto const& [id,rowValue] : this->fieldValues) {
                  if (this->dataType == "INT" || this->dataType == "BOOL") {
                        if (compare(toInt(value),toInt(rowValue))) {
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


public:
      Column(std::string  name, std::string  dataType):name(std::move(name)),dataType(std::move(dataType)) {};
      ~Column() = default;
      [[nodiscard]] auto findValue(std::string const& value) const;
      [[nodiscard]] auto getFilteredRows(std::string const& value,std::string const& operand) const-> std::vector<int>;
      auto insertValue(int const& id,std::string const& value)->void;
      auto getName() -> std::string;
      auto setName(std::string const& newName)->void;
      auto eraseFieldValues()->void;
      auto printAllRows()->std::string;
      auto printFilteredRows(std::vector<int> const& ids)->void;
      auto updateValue(int const& id, std::string const& newValue)->void;

};


