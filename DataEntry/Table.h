
#include <string>
#include <map>
#include <vector>
#ifndef TABLE_H
#define TABLE_H
struct Table{
    std::string name;
    std::vector<std::string> columns;
    std::map<int, std::vector<std::pair<std::string,std::string>>> rows;
    Table(std::string const& name, std::vector<std::string> columns);
};






#endif
