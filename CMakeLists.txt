cmake_minimum_required(VERSION 3.30)
project(GalaxyQuery)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(GalaxyQuery
        main.cpp
        DataEntry/DB.cpp
        DataEntry/Table.cpp
        DataEntry/Column.cpp
        QueryEngine/InterpreterQuery.cpp
        DataEntry/GalaxyQueryExporter.cpp
)

target_include_directories(
        GalaxyQuery PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
