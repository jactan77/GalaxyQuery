
#include "DDL.h"

auto DDL_FUNC::createDB(std::string const& name) -> Db{
       return Db(name);
};

