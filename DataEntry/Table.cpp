

#include "Table.h"
Table::Table(std::string const& name, std::map<std::string, std::string> const& columns){
             this->name = name;
             this->columns = columns;
             this->rows = std::map<int, std::vector<std::pair<std::string,std::string>>>();

}
