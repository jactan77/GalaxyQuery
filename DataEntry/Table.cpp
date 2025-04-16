

#include "Table.h"
Table::Table(std::string const& name, std::vector<std::string> columns){
             this->name = name;
             this->columns = columns;
             this->rows = std::map<int, std::vector<std::pair<std::string,std::string>>>();

}
