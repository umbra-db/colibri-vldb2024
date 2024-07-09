#include "duckdb/DuckDB.hpp"
#include "duckdb/DuckDBConnectionConfig.hpp"
#include "duckdb/DuckDBDriver.hpp"
#include "duckdb/DuckDBInterface.hpp"
#include "duckdb/hattrick/HattrickDriver.hpp"
#include "duckdb/tpch/TpchDriver.hpp"
#include "txbench/DriverConfig.hpp"
#include "txbench/Exception.hpp"
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
DuckDB::DuckDB(System database, Benchmark benchmark, filesystem::path scriptsPath, filesystem::path logPath, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients)
   : Database(database, benchmark, move(scriptsPath), move(logPath), scaleFactor, transactionalClients, analyticalClients)
// Constructor
{
}
//---------------------------------------------------------------------------
DuckDB::~DuckDB()
// Deconstructor
{
}
//---------------------------------------------------------------------------
void DuckDB::executeQuery(const std::string& query)
// Execute a query
{
   connection->exec(query);
}
//---------------------------------------------------------------------------
void DuckDB::connect()
// Connect to the database
{
   database = make_unique<DuckDBDatabase>("/db/duckdb.db");
   connection = make_unique<DuckDBConnection>(*database);

   {
      char* limit = getenv("MEMORYLIMIT");
      if (limit != nullptr) {
         connection->exec("set global memory_limit = '" + string(limit) + "';");
         cerr << "memory limit set to " << limit << endl;
      }
   }

   cerr << "setting up database..." << endl;
   for (const auto& filename : getBenchmarkConfig().getInitScripts())
      RETHROW(executeScript(filename), "unable to execute init script \"" + filename + "\": " + e.getMessage());

   connection = openConnection();
}
//---------------------------------------------------------------------------
std::unique_ptr<DuckDBConnection> DuckDB::openConnection() const
// Open a connection
{
   return make_unique<DuckDBConnection>(*database, getDatabaseName());
}
//---------------------------------------------------------------------------
unique_ptr<ConnectionConfig> DuckDB::prepareConnectionConfig() const
// Prepare the connection config
{
   return make_unique<DuckDBConnectionConfig>();
}
//---------------------------------------------------------------------------
std::unique_ptr<DatabaseDriver> DuckDB::prepareDriver(const string& driverConfigFile, const std::string& logfile)
// Prepare teh database driver
{
   auto driverConfig = make_unique<DriverConfig>();
   if (auto res = driverConfig->read(driverConfigFile); !res)
      throw Exception("unable to read driver config at \"" + driverConfigFile + "\": " + res.getMessage());

   switch (benchmark.get()) {
      case Benchmark::TPCH: return make_unique<TpchDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::TPCH1000: return make_unique<TpchDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::HATTRICK: return make_unique<HattrickDriver>(*this, std::move(driverConfig), std::move(logfile));
      default: throw Exception("benchmark \"" + benchmark.toString() + "\" not supported");
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
