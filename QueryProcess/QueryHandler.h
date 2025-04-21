//
// Created by budix on 4/22/2025.
//
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "../DataEntry/DB.h"
#include "../DataEntry/Table.h"
#ifndef QUERYHANDLER_H
#define QUERYHANDLER_H



class QueryHandler {

    public:
        static auto processCreate(std::vector<std::string> createQuery)-> void;
        static auto processInsert(std::vector<std::string> insertQuery)-> void;
        static auto processUpdate(std::vector<std::string> updateQuery)-> void;
        static auto processDelete(std::vector<std::string> deleteQuery)-> void;
        static auto processSelect(std::vector<std::string> selectQuery)-> void;
        static auto processAlter(std::vector<std::string> alterQuery)-> void;
        static auto processDrop(std::vector<std::string> dropQuery)-> void;


};



#endif //QUERYHANDLER_H
