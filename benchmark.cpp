#include "DB.h"
#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <vector>

class Benchmark {
public:
  struct Result {
    std::string name;
    double totalMs;
    size_t operations;
    double opsPerSec;
  };

  template <typename Func>
  static auto measure(const std::string &name, size_t iterations, Func &&func)
      -> Result {
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < iterations; ++i) {
      func(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration<double, std::milli>(end - start).count();
    return Result{name, duration, iterations, (iterations / duration) * 1000.0};
  }

  static auto printResult(const Result &r) -> void {
    std::cout << std::format("| {:25} | {:10} | {:12.2f} ms | {:12.0f} ops/s |",
                             r.name, r.operations, r.totalMs, r.opsPerSec)
              << std::endl;
  }

  static auto printHeader() -> void {
    std::cout << "+---------------------------+------------+----------------+--"
                 "--------------+"
              << std::endl;
    std::cout << std::format("| {:25} | {:10} | {:14} | {:14} |", "Operation",
                             "Count", "Total Time", "Throughput")
              << std::endl;
    std::cout << "+---------------------------+------------+----------------+--"
                 "--------------+"
              << std::endl;
  }

  static auto printFooter() -> void {
    std::cout << "+---------------------------+------------+----------------+--"
                 "--------------+"
              << std::endl;
  }
};

auto benchmarkInsert(Db &db, size_t count) -> Benchmark::Result {
  db.processCreateTable("BENCH_INSERT",
                        {{"ID", "INT"}, {"NAME", "STRING"}, {"VALUE", "INT"}});

  std::cout.setstate(std::ios_base::failbit);
  auto result = Benchmark::measure("INSERT", count, [&](size_t i) {
    std::map<std::string, std::string> values = {
        {"ID", std::to_string(i + 1)},
        {"NAME", "USER" + std::to_string(i)},
        {"VALUE", std::to_string(i * 10)}};
    db.processInsert("BENCH_INSERT", values);
  });
  std::cout.clear();
  return result;
}

auto benchmarkSelect(Db &db, size_t count) -> Benchmark::Result {
  std::cout.setstate(std::ios_base::failbit);
  auto result = Benchmark::measure("SELECT *", count, [&](size_t) {
    db.processSelect("BENCH_INSERT", {"*"});
  });
  std::cout.clear();
  return result;
}

auto benchmarkSelectWhere(Db &db, size_t count) -> Benchmark::Result {
  std::cout.setstate(std::ios_base::failbit);
  auto result = Benchmark::measure("SELECT WHERE", count, [&](size_t i) {
    db.processSelect("BENCH_INSERT", {"*"},
                     {"VALUE", ">", std::to_string(i % 100)});
  });
  std::cout.clear();
  return result;
}

auto benchmarkUpdate(Db &db, size_t count) -> Benchmark::Result {
  std::cout.setstate(std::ios_base::failbit);
  auto result = Benchmark::measure("UPDATE WHERE", count, [&](size_t i) {
    db.processUpdate("BENCH_INSERT", {{"VALUE", std::to_string(i * 20)}},
                     {"ID", "=", std::to_string((i % 1000) + 1)});
  });
  std::cout.clear();
  return result;
}

auto benchmarkTableCreation(size_t count) -> Benchmark::Result {
  auto db = std::make_unique<Db>("BenchDB");
  std::cout.setstate(std::ios_base::failbit);
  auto result = Benchmark::measure("CREATE TABLE", count, [&](size_t i) {
    db->processCreateTable("TABLE_" + std::to_string(i),
                           {{"COL1", "INT"}, {"COL2", "STRING"}});
  });
  std::cout.clear();
  return result;
}

auto benchmarkColumnAdd(Db &db, size_t count) -> Benchmark::Result {
  db.processCreateTable("BENCH_ALTER", {{"ID", "INT"}});
  std::cout.setstate(std::ios_base::failbit);
  auto result = Benchmark::measure("ALTER ADD COLUMN", count, [&](size_t i) {
    db.processAlterAdd("BENCH_ALTER", "COL_" + std::to_string(i), "STRING");
  });
  std::cout.clear();
  return result;
}

auto main() -> int {
  std::cout << R"(
  ____                  _                          _    
 | __ )  ___ _ __   ___| |__  _ __ ___   __ _ _ __| | __
 |  _ \ / _ \ '_ \ / __| '_ \| '_ ` _ \ / _` | '__| |/ /
 | |_) |  __/ | | | (__| | | | | | | | | (_| | |  |   < 
 |____/ \___|_| |_|\___|_| |_|_| |_| |_|\__,_|_|  |_|\_\
                                                        
    GalaxyQuery Performance Benchmark
)" << std::endl;

  std::vector<Benchmark::Result> results;

  std::cout << "Running CREATE TABLE benchmark (100 tables)..." << std::endl;
  results.push_back(benchmarkTableCreation(100));

  {
    auto db = std::make_unique<Db>("BenchDB");

    std::cout << "Running INSERT benchmark (1000 rows)..." << std::endl;
    results.push_back(benchmarkInsert(*db, 1000));

    std::cout << "Running SELECT * benchmark (100 queries)..." << std::endl;
    results.push_back(benchmarkSelect(*db, 100));

    std::cout << "Running SELECT WHERE benchmark (100 queries)..." << std::endl;
    results.push_back(benchmarkSelectWhere(*db, 100));

    std::cout << "Running UPDATE benchmark (500 updates)..." << std::endl;
    results.push_back(benchmarkUpdate(*db, 500));
  }

  {
    auto db = std::make_unique<Db>("BenchDB");
    std::cout << "Running ALTER ADD COLUMN benchmark (50 columns)..."
              << std::endl;
    results.push_back(benchmarkColumnAdd(*db, 50));
  }

  std::cout << std::endl
            << "=== BENCHMARK RESULTS ===" << std::endl
            << std::endl;

  Benchmark::printHeader();
  for (const auto &r : results) {
    Benchmark::printResult(r);
  }
  Benchmark::printFooter();

  std::cout << std::endl;
  return 0;
}
