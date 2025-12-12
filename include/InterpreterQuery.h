#pragma once
#include "DB.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <ranges>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class InterpreterQuery {
public:
  static auto processQuery(std::unique_ptr<Db> &db, std::string const &input)
      -> void;

private:
  using QueryHandler =
      std::function<void(Db &, const std::vector<std::string> &)>;
  using DropHandler = std::function<void(std::unique_ptr<Db> &,
                                         const std::vector<std::string> &)>;

  static auto getTokens(std::string element) -> std::vector<std::string>;
  static auto tokenizeConditions(std::string const &parseCondition)
      -> std::vector<std::string>;
  static auto tokenizeColumns(std::string const &parseColumns)
      -> std::vector<std::string>;

  static auto handleSelect(Db &db, const std::vector<std::string> &query)
      -> void;
  static auto handleInsert(Db &db, const std::vector<std::string> &query)
      -> void;
  static auto handleUpdate(Db &db, const std::vector<std::string> &query)
      -> void;
  static auto handleCreate(Db &db, const std::vector<std::string> &query)
      -> void;
  static auto handleAlter(Db &db, const std::vector<std::string> &query)
      -> void;
  static auto handleDelete(Db &db, const std::vector<std::string> &query)
      -> void;
  static auto handleDrop(std::unique_ptr<Db> &db,
                         const std::vector<std::string> &query) -> void;

  static inline const std::unordered_map<std::string, QueryHandler>
      queryHandlers = {{"SELECT", handleSelect}, {"INSERT", handleInsert},
                       {"UPDATE", handleUpdate}, {"CREATE", handleCreate},
                       {"ALTER", handleAlter},   {"DELETE", handleDelete}};

  static inline const std::unordered_set<std::string> validDataTypes = {
      "INT", "STRING", "CHAR", "BOOL"};

  static auto isValidDataType(const std::string &type) -> bool {
    return validDataTypes.find(type) != validDataTypes.end();
  }
};
