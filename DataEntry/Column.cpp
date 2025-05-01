

#include "Column.h"

#include <algorithm>
#include <ranges>
auto Column::insertValue(int const& id,std::string const& value)->void {
    if (dataType == selectType(value)) {
        auto const it = fieldValues.find(id);
            if (it != fieldValues.end()) {
                throw std::runtime_error("Attempted to insert a row that conflicts with an existing entry. Duplicate entries are not allowed.");
            }
        fieldValues.insert({id, value});
        return;
    }
                throw std::runtime_error(std::format("The provided value {} is incompatible with the data type of the column {}.",value,this->name));

}
auto Column::setName(std::string const& newName)->void {
    this->name=newName;
}

auto Column::getName() -> std::string {
    return name;
};
auto Column::eraseFieldValues()->void {
    fieldValues.clear();
};
auto Column::findValue(std::string const& value) const {
    return std::ranges::find_if(this->fieldValues,[value](auto const& pair)-> bool {
        return value == pair.second;
    });
};

auto Column::selectType(std::string const& value)->std::string {
    auto isInt = [](std::string const& str)-> bool {
        const auto isNegative = str.at(0) == '-' ? 1 : 0;
        return std::all_of(str.begin()+isNegative,str.end(),::isdigit);
    };
    if (isInt(value)) {
        return "INT";
    }
    if (value == "TRUE" or value == "FALSE") {
        return "BOOL";
    }
    if (value.size() == 1) {
        return "CHAR";
    }
    return "STRING";

}
