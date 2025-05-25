

#include "Column.h"
auto Column::insertValue(int const& id,std::string const& value)->void {
    if (dataType == selectType(value)) {
        auto const it = fieldValues.find(id);
            if (it != fieldValues.end()) {
                throw std::runtime_error("Attempted to insert a row that conflicts with an existing entry. Duplicate entries are not allowed.");
            }
        fieldValues.insert({id, value});
        return;
    }
    std::cout << fieldValues.size() << std::endl;
    throw std::runtime_error(std::format("The provided value {} is incompatible with the data type of the column {}.",value,this->name));

}
auto Column::insertDefaultValues(const int ids)->void {
    auto const& defaultValue = defaultValues[this->dataType];
    for (int id = 1; id < ids; id++) {
        fieldValues.insert({id,defaultValue});
    }
}
auto Column::setName(std::string const& newName)->void {
    this->name=newName;
}

auto Column::getName() const -> std::string {
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
    static const std::unordered_map<std::string, std::function<std::vector<int>(const Column&, const std::string&)>> operators = {
        {"=",  [](const Column& column, const std::string& val) { return column == val; }},
        {"!=", [](const Column& column, const std::string& val) { return column != val; }},
        {">",  [](const Column& column, const std::string& val) { return column > val; }},
        {"<",  [](const Column& column, const std::string& val) { return column < val; }},
        {">=", [](const Column& column, const std::string& val) { return column >= val; }},
        {"<=", [](const Column& column, const std::string& val) { return column <= val; }}
    };
    auto const it = operators.find(operand);
    if (it  == operators.end()) {
        throw std::runtime_error("An invalid operator was used in the query");
    }
    return it->second(*this,value);
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

auto Column::setFieldValues(const std::map<int,std::string> &newFieldValues)->void {
    this->fieldValues=newFieldValues;
}
auto Column::getOrderedRows() const -> std::vector<int> {
    auto comp = [this](std::pair<int,std::string> const& p1, std::pair<int,std::string> const& p2)->bool {
        if (this->dataType == "INT") {
            return toInt(p1.second) > toInt(p2.second);
        }
        return p1.second > p2.second;
    };
    auto ids = std::vector<std::pair<int,std::string>>();
    for (auto const& pair: this->fieldValues) {
        ids.emplace_back(pair);
    }
    std::ranges::sort(ids,comp);
    auto orderedIds = std::vector<int>();
    for (const auto &key: ids | std::views::keys) {
        orderedIds.push_back(key);
    }
    std::ranges::reverse(orderedIds);
    return orderedIds;
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
