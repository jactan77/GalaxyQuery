cmake_minimum_required(VERSION 3.30)
project(GalaxiQuery)

set(CMAKE_CXX_STANDARD 20)

add_executable(GalaxyQuery main.cpp DataEntry/DB.cpp QueryEngine/InterpreterQuery.cpp QueryTokens/DDL.cpp)
