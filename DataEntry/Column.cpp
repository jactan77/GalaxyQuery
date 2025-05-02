

#include "Column.h"

#include <algorithm>
#include <ranges>
template<typename Comparator>
auto Column::FilterByColumn(const Column& other,Comparator compare) const -> std::vector<int> {
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
auto Column::FilterByValue(std::string const& value, Comparator compare) const -> std::vector<int> {
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

auto Column::toInt(std::string const& value)->int{
    if (selectType(value) == "BOOL") {
        return value == "TRUE" ?  1 : 0;
    }
    return std::stoi(value);
}
