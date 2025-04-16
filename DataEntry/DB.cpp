

#include "DB.h"

Db::Db(std::string const& name){
    this->name = name;
    this->tables = std::vector<Table>();
}
