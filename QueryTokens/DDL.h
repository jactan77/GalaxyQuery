

#ifndef DDL_H
#define DDL_H



#include <string>
#include <map>
#include <vector>
#include "../DataEntry/DB.h"
#include "../DataEntry/Table.h"

enum class DDLTOKENS {
    CREATE_TABLE,
    ALTER_TABLE,
    ADD,
    DROP_COLUMN,
    DROP_TABLE,
    CREATE_DATABASE,
    RENAME_COLUMN,
    RENAME_TABLE


};

class DDL_FUNC{
      public:
        static Db createDB(std::string const& name);
        static void dropTable(Db& db);
        static void alterTable(Db& db, Table& table);


};





#endif //DDL_H
