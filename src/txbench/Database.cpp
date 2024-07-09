#include "txbench/Database.hpp"
#include "duckdb/DuckDB.hpp"
#include "postgres/Postgres.hpp"
#include "singlestore/SingleStore.hpp"
#include "tidb/TiDB.hpp"
#include "txbench/DatabaseDriver.hpp"
#include "txbench/Exception.hpp"
#include "txbench/tpcc/TpccConfig.hpp"
#include "umbra/Umbra.hpp"
#include <fstream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
Benchmark Benchmark::create(const string& benchmark)
// Create a benchmark
{
   if (benchmark == "LOAD") {
      return Benchmark(LOAD);
   } else if (benchmark == "TATP") {
      return Benchmark(TATP);
   } else if (benchmark == "TPCC") {
      return Benchmark(TPCC);
   } else if (benchmark == "TPCH") {
      return Benchmark(TPCH);
   } else if (benchmark == "TPCH1000") {
      return Benchmark(TPCH1000);
   } else if (benchmark == "TPCCH") {
      return Benchmark(TPCCH);
   } else if (benchmark == "HATTRICK") {
      return Benchmark(HATTRICK);
   } else if (benchmark == "HYBENCH") {
      return Benchmark(HYBENCH);
   } else {
      throw Exception("unable to parse benchmark \"" + benchmark + "\"");
   }
}
//---------------------------------------------------------------------------
std::string Benchmark::toString() const
// To string
{
   switch (value) {
      case LOAD: return "LOAD";
      case TATP: return "TATP";
      case TPCC: return "TPCC";
      case TPCH: return "TPCH";
      case TPCH1000: return "TPCH1000";
      case TPCCH: return "TPCCH";
      case HATTRICK: return "HATTRICK";
      case HYBENCH: return "HYBENCH";
   }
   __builtin_unreachable();
}
//---------------------------------------------------------------------------
std::string Benchmark::toScriptsPath() const
// To path
{
   switch (value) {
      case LOAD: return "load";
      case TATP: return "tatp";
      case TPCC: return "tpcc";
      case TPCH: return "tpch";
      case TPCH1000: return "tpch1000";
      case TPCCH: return "tpcch";
      case HATTRICK: return "hattrick";
      case HYBENCH: return "hybench";
   }
   __builtin_unreachable();
}
//---------------------------------------------------------------------------
std::string Benchmark::toPath() const
// To path
{
   switch (value) {
      case LOAD: return "load";
      case TATP: return "tatp";
      case TPCC: return "tpcc";
      case TPCH: return "tpch";
      case TPCH1000: return "tpch";
      case TPCCH: return "tpcch";
      case HATTRICK: return "hattrick";
      case HYBENCH: return "hybench";
   }
   __builtin_unreachable();
}
//---------------------------------------------------------------------------
Database::Database(System database, Benchmark benchmark, std::filesystem::path scriptsPath, std::filesystem::path logPath, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients)
   : database(database), benchmark(benchmark), scriptsPath(move(scriptsPath)), logPath(move(logPath)), scaleFactor(scaleFactor), transactionalClients(transactionalClients), analyticalClients(analyticalClients)
// Constructor
{
}
//---------------------------------------------------------------------------
Database::~Database()
// Destructor
{
   Database::shutdownDatabase();
}
//---------------------------------------------------------------------------
void Database::init()
// Initialise
{
   connectionConfig = prepareConnectionConfig();
   benchmarkConfig = prepareBenchmarkConfig();

   if (auto res = connectionConfig->read(getConnectionConfigFile()); !res)
      throw Exception("unable to read connection config from \"" + getConnectionConfigFile().string() + "\": " + res.getMessage());

   if (auto res = benchmarkConfig->read(getBenchmarkConfigFile()); !res)
      throw Exception("unable to read connection config from \"" + getBenchmarkConfigFile().string() + "\": " + res.getMessage());

   startDatabase();
}
//---------------------------------------------------------------------------
filesystem::path Database::getConnectionConfigFile() const
// Get the connection config file
{
   return filesystem::path(scriptsPath).append("connection-config.json");
}
//---------------------------------------------------------------------------
std::filesystem::path Database::getBenchmarkConfigFile() const
// Get the benchmark config file
{
   return filesystem::path(scriptsPath).append(benchmark.toScriptsPath()).append("benchmark-config.json");
}
//---------------------------------------------------------------------------
std::filesystem::path Database::getLogFile() const
// Get the log file path
{
   return filesystem::path(logPath).append(to_string(scaleFactor) + "_" + to_string(transactionalClients) + "_" + to_string(analyticalClients));
}
//---------------------------------------------------------------------------
std::unique_ptr<BenchmarkConfig> Database::prepareBenchmarkConfig() const
// Prepare the benchmark config
{
   if (benchmark.get() == Benchmark::TPCC || benchmark.get() == Benchmark::TPCCH)
      return make_unique<tpcc::TpccConfig>();

   return make_unique<BenchmarkConfig>();
}
//---------------------------------------------------------------------------
void Database::startDatabase()
// Start the database
{
}
//---------------------------------------------------------------------------
void Database::shutdownDatabase()
// Shutdown the database
{
}
//---------------------------------------------------------------------------
void Database::readScript(const string& filename, string& target) const
// Read a query script from a file
{
   auto sqlFilePath = filesystem::path(scriptsPath).append(benchmark.toScriptsPath()).append(filename);
   ifstream t(sqlFilePath);
   if (!t.is_open())
      throw Exception("unable to open \"" + sqlFilePath.string() + "\"");

   target = string((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
}
//---------------------------------------------------------------------------
void Database::executeScript(const string& filename)
// Execute a script
{
   string script;
   readScript(filename, script);
   executeQuery(script);
}
//---------------------------------------------------------------------------
void Database::execute(const std::string& driverConfigFile)
// Execute a benchmark
{
   // Setup the benchmark driver
   auto driver = prepareDriver(driverConfigFile, getLogFile());

   driver->prepareDatabase();

   if (!driver->run())
      throw Exception("could not run benchmark \"" + benchmark.toString() + "\"");
}
//---------------------------------------------------------------------------
unique_ptr<Database> Database::create(const string& database, const string& instanceName, Benchmark benchmark, const filesystem::path& txbenchDir, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients)
// Create a database
{
   auto scriptsPath = filesystem::path(txbenchDir).append("scripts");
   auto logPath = filesystem::path(txbenchDir).append("output").append(instanceName).append(benchmark.toPath());

   unique_ptr<Database> db;
   if (database == "AlloyDB") {
      db = make_unique<postgres::Postgres>(AlloyDB, benchmark, scriptsPath.append("alloydb"), move(logPath), scaleFactor, transactionalClients, analyticalClients);
   } else if (database == "DuckDB") {
      db = make_unique<duckdb::DuckDB>(DuckDB, benchmark, scriptsPath.append("duckdb"), move(logPath), scaleFactor, transactionalClients, analyticalClients);
   } else if (database == "SingleStore") {
      db = make_unique<singlestore::SingleStore>(SingleStore, benchmark, scriptsPath.append("singlestore"), move(logPath), scaleFactor, transactionalClients, analyticalClients);
   } else if (database == "TiDB") {
      db = make_unique<tidb::TiDB>(TiDB, benchmark, scriptsPath.append("tidb"), move(logPath), scaleFactor, transactionalClients, analyticalClients);
   } else if (database == "Postgres") {
      db = make_unique<postgres::Postgres>(Postgres, benchmark, scriptsPath.append("postgres"), move(logPath), scaleFactor, transactionalClients, analyticalClients);
   } else if (database == "Umbra") {
      db = make_unique<umbra::Umbra>(Umbra, benchmark, scriptsPath.append("umbra"), move(logPath), scaleFactor, transactionalClients, analyticalClients);
   } else if (database == "UmbraLocal") {
      db = make_unique<umbra::Umbra>(UmbraLocal, benchmark, scriptsPath.append("umbra"), move(logPath), scaleFactor, transactionalClients, analyticalClients);
   } else {
      throw Exception("unknown database \"" + database + "\"");
   }

   db->init();
   return db;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
