#include "singlestore/SingleStore.hpp"
#include "singlestore/SingleStoreConnectionConfig.hpp"
#include "singlestore/SingleStoreDriver.hpp"
#include "singlestore/hattrick/HattrickDriver.hpp"
#include "singlestore/tatp/TatpDriver.hpp"
#include "singlestore/tpcc/TpccDriver.hpp"
#include "singlestore/tpcch/TpcchDriver.hpp"
#include "singlestore/tpch/TpchDriver.hpp"
#include "txbench/DriverConfig.hpp"
#include "txbench/Exception.hpp"
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
SingleStore::SingleStore(System database, Benchmark benchmark, filesystem::path scriptsPath, filesystem::path logPath, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients)
   : Database(database, benchmark, move(scriptsPath), move(logPath), scaleFactor, transactionalClients, analyticalClients)
// Constructor
{
}
//---------------------------------------------------------------------------
void SingleStore::executeQuery(const std::string& query)
// Execute a query
{
   connection.exec(query);
}
//---------------------------------------------------------------------------
void SingleStore::connect()
// Connect to the database
{
   environment.allocate();
   {
      // SingleStore takes some time to startup
      this_thread::sleep_for(10s);

      // Perform initial database setup
      RETHROW(connection.connect(environment, getConnectionString("memsql")), "unable to connect to memsql database: " + e.getMessage() + "\n" + getConnectionString("memsql"));

      cerr << "setting up database..." << endl;
      for (const auto& filename : getBenchmarkConfig().getInitScripts())
         RETHROW(executeScript(filename), "unable to execute init script \"" + filename + "\": " + e.getMessage());
   }

   // Connect to the benchmark database
   RETHROW(connection.disconnect(), "unable to disconnect from master database: " + e.getMessage());
   RETHROW(connection.connect(environment, getConnectionString()), "unable to connect to benchmark database: " + e.getMessage() + "\n" + getConnectionString());
}
//---------------------------------------------------------------------------
unique_ptr<ConnectionConfig> SingleStore::prepareConnectionConfig() const
// Prepare the connection config
{
   return make_unique<SingleStoreConnectionConfig>();
}
//---------------------------------------------------------------------------
std::unique_ptr<DatabaseDriver> SingleStore::prepareDriver(const string& driverConfigFile, const std::string& logfile)
// Prepare teh database driver
{
   auto driverConfig = make_unique<DriverConfig>();
   if (auto res = driverConfig->read(driverConfigFile); !res)
      throw Exception("unable to read driver config at \"" + driverConfigFile + "\": " + res.getMessage());

   switch (benchmark.get()) {
      case Benchmark::TPCC: return make_unique<TpccDriver>(*this, environment, std::move(driverConfig), std::move(logfile));
      case Benchmark::TPCCH: return make_unique<TpcchDriver>(*this, environment, std::move(driverConfig), std::move(logfile));
      case Benchmark::TPCH: return make_unique<TpchDriver>(*this, environment, std::move(driverConfig), std::move(logfile));
      case Benchmark::TPCH1000: return make_unique<TpchDriver>(*this, environment, std::move(driverConfig), std::move(logfile));
      case Benchmark::TATP: return make_unique<TatpDriver>(*this, environment, std::move(driverConfig), std::move(logfile));
      case Benchmark::HATTRICK: return make_unique<HattrickDriver>(*this, environment, std::move(driverConfig), std::move(logfile));
      default: throw Exception("benchmark \"" + benchmark.toString() + "\" not supported");
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
