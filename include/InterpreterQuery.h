#pragma once
#include <iostream>
#include <algorithm>
#include <functional>
#include <ranges>

#include "DB.h"
#include <string>
#include <vector>
#include <map>
#include <regex>




class InterpreterQuery {
	static 	auto getTokens				(std::string element)->std::vector<std::string>;
    static 	auto initializeCreate		(std::string createStatement) -> bool;
    static	auto tokenizeSelectQuery	(Db*& db, const std::vector<std::string>& query) -> void;
	static	auto tokenizeInsertQuery	(Db*& db, const std::vector<std::string>& query) -> void;
	static	auto tokenizeUpdateQuery	(Db*& db, const std::vector<std::string>& query) -> void;
	static	auto tokenizeConditions		(std::string const &parseCondition) -> std::vector<std::string>;

	static	auto tokenizeColumns(std::string const &parseColumns) -> std::vector<std::string>;


	static	auto tokenizeCreateQuery	(Db*& db, const std::vector<std::string>& query) -> void;
	static	auto tokenizeDropQuery   	(Db*& db, const std::vector<std::string>& query) -> void;
	static	auto tokenizeAlterQuery  	(Db*& db, const std::vector<std::string>& query) -> void;
	static	auto tokenizeDeleteQuery	(Db*& db, const std::vector<std::string>& query) -> void;
	static	inline auto dataTypes = std::vector<std::string>{
        "INT","STRING", "CHAR", "BOOL"
	};
    static inline auto const GalaxyKeywords = std::map<std::string, std::function<void(Db*& db, const std::vector<std::string>& query)>>{
		{"SELECT", tokenizeSelectQuery},
    	{"INSERT", tokenizeInsertQuery},
		{"UPDATE", tokenizeUpdateQuery},
		{"CREATE", tokenizeCreateQuery},
		{"DROP",	tokenizeDropQuery},
		{"ALTER",  tokenizeAlterQuery},
		{"DELETE", tokenizeDeleteQuery}
    };
  	public:
          static auto processQuery(Db*& db,std::string const& input)->void;
};





