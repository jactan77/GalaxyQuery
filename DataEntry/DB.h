

#ifndef DB_H
#define DB_H
#include <string>
#include <map>
#include <vector>
#include "Table.h"
struct Db{
    std::string name;
    std::vector<Table> tables;
    explicit Db(std::string const& name);




};





#endif