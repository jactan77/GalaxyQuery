

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
auto Column::getFilteredRows(std::string const& value,std::string const& operand) const-> std::vector<int> {
    switch (operand[0]) {
        case '=': {
            return *this == value;
        }
        case '>':
            return *this > value;
        case '<':
            return *this < value;
        default:
            throw std::runtime_error("An invalid operator was used in the query");
    }

}
auto Column::updateValue(int const& id, std::string const& newValue) -> void {
    if (selectType(newValue) != this->dataType) {
        throw std::runtime_error(std::format("The provided value {} is incompatible with the data type of the column {}.",newValue,this->name));
    }
    this->fieldValues[id] = newValue;
}
auto Column::printHeader(const size_t width)-> std::string {
    std::stringstream header;
    header << "| " << this->getName();
    for (size_t i = this->getName().size(); i < width; i++) {
        header << " ";
    }
    header << " |" << '\n';
    return header.str();
}

auto Column::printRow(std::string const& value, const size_t width)-> std::string{
    std::stringstream row;
    row << "| " << value;
    for (size_t i = value.size(); i < width+1; i++) {
        row << " ";
    }
    row << "|\n";

    return row.str();
}

auto Column::calculateWidth() const -> size_t {
    size_t width = this->name.size();
    for (auto const& [id, value]: this->fieldValues) {
        width = std::max(width, value.size());
    }
    return width;
}

auto Column::printRows(const std::set<int>& ids)  -> std::string {
    std::stringstream result;
    const size_t width = calculateWidth();
    const std::string line = "+" + std::string(width + 2, '-') + "+";

    result << line << std::endl;
    result << printHeader(width);
    result << line << std::endl;

    for (auto const& [id, value]: this->fieldValues) {
        if (ids.contains(id)) {
            result << printRow(value, width);
        }
    }

    result << line << std::endl;

    return result.str();
}



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
