#pragma once
#include <string>
#include <map>
#include <vector>
#ifndef TABLE_H
#define TABLE_H
struct Table{
    std::string name;
    std::map<std::string, std::string> columns; //  {name, dataType}
    std::map<int, std::vector<std::pair<std::string,std::string>>> rows; // // {{id},{<colum1:data>, <colum1:data>},.....}
    Table(std::string const& name, std::map<std::string, std::string> columns);
};






#endif
