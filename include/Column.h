
#pragma once

#include <map>
#include <string>
#include <ranges>
#include <format>
#include <utility>
#include <vector>


class Column {
      std::string name;
      std::string dataType;
      std::map<int,std::string> fieldValues; // key <=> id
      static auto selectType(std::string const& )->std::string;
public:
      Column(std::string  name, std::string  dataType):name(std::move(name)),dataType(std::move(dataType)) {};
      ~Column() = default;
      auto insertValue(int const& id,std::string const& value)->void;
      auto getName() -> std::string;
      auto setName(std::string const& newName)->void;
      auto eraseFieldValues()->void;



};


