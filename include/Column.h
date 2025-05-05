
#pragma once

#include <map>
#include <string>
#include <ranges>
#include <format>
#include <utility>
#include <functional>
#include <vector>


class Column {
      std::string name;
      std::string dataType;
      std::map<int,std::string> fieldValues; // key <=> id
      static auto selectType(std::string const& value )->std::string;
      static auto toInt(std::string const& value)->int;

      template<typename Comparator>
      [[nodiscard]] auto FilterByColumn(const Column& other,Comparator compare) const -> std::vector<int> {
            if (this->dataType != other.dataType) {
                  throw std::runtime_error("You are attempting to compare columns with incompatible data types");
            }
            auto filteredIds = std::vector<int>();

            for (auto id = 1; id <= other.fieldValues.size(); id++ ) {
                  if (this->dataType == "INT" || this->dataType == "BOOL") {
                        if (compare(toInt(this->fieldValues.at(id)),toInt(other.fieldValues.at(id)))) {
                              filteredIds.push_back(id);
                        }
                  } else {
                        if (compare(this->fieldValues.at(id),other.fieldValues.at(id))) {
                              filteredIds.push_back(id);
                        }
                  }
            }
            return filteredIds;

      };

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


      auto operator==(const Column& other ) const ->std::vector<int> {
            return FilterByColumn(other,[](const auto& val1, const auto& val2)->bool{return val1 == val2;});
      }
      auto operator==(const std::string& value) const -> std::vector<int> {
            return FilterByValue(value,[](const auto& val1, const auto& val2)->bool{return val1 == val2;});
      }
      auto operator>(const Column& other ) const ->std::vector<int> {
            return FilterByColumn(other,[](const auto& val1, const auto& val2)->bool{return val1 > val2;});
      }
      auto operator>(const std::string& value) const -> std::vector<int> {
            return FilterByValue(value,[](const auto& val1, const auto& val2)->bool{return val1 > val2;});
      }
      auto operator<(const Column& other ) const ->std::vector<int> {
            return FilterByColumn(other,[](const auto& val1, const auto& val2)->bool{return val1 < val2;});
      }
      auto operator<(const std::string& value) const -> std::vector<int> {
            return FilterByValue(value,[](const auto& val1, const auto& val2)->bool{return val1 < val2;});
      }


public:
      Column(std::string  name, std::string  dataType):name(std::move(name)),dataType(std::move(dataType)) {};
      ~Column() = default;
      auto insertValue(int const& id,std::string const& value)->void;
      auto getName() -> std::string;
      auto setName(std::string const& newName)->void;
      auto eraseFieldValues()->void;
      [[nodiscard]] auto findValue(std::string const& value) const;
      auto checkCondition()->bool;

};


