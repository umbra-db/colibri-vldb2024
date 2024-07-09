#include "tidb/TiDB.hpp"
#include "tidb/TiDBDriver.hpp"
#include "tidb/hattrick/HattrickDriver.hpp"
#include "tidb/tpch/TpchDriver.hpp"
#include "txbench/DriverConfig.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/OdbcConnectionConfig.hpp"
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::tidb {
//---------------------------------------------------------------------------
TiDB::TiDB(System database, Benchmark benchmark, filesystem::path scriptsPath, filesystem::path logPath, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients)
   : Database(database, benchmark, move(scriptsPath), move(logPath), scaleFactor, transactionalClients, analyticalClients)
// Constructor
{
}
//---------------------------------------------------------------------------
void TiDB::executeQuery(const std::string& query)
// Execute a query
{
   connection.exec(query);
}
//---------------------------------------------------------------------------
void TiDB::connect()
// Connect to the database
{
   environment.allocate();
   {
      this_thread::sleep_for(10s);

      // Perform initial database setup
      RETHROW(connection.connect(environment, getConnectionString("test")), "unable to connect to tidb database: " + e.getMessage() + "\n" + getConnectionString("memsql"));

      executeQuery("drop table if exists test;");
      executeQuery("create table test (t int);");
      for (unsigned tries = 0; true; tries++) {
         try {
            executeQuery("alter table test set tiflash replica 1");
            break;
         } catch (Exception& e) {
            cerr << "tiflash not ready, retrying!" << endl;
         }

         if (tries > 120) throw Exception("TiFlash not running!");
         this_thread::sleep_for(1s);
      }
      executeQuery("drop table test;");

      cerr << "setting up database..." << endl;
      for (const auto& filename : getBenchmarkConfig().getInitScripts())
         RETHROW(executeScript(filename), "unable to execute init script \"" + filename + "\": " + e.getMessage());
   }

   // Connect to the benchmark database
   RETHROW(connection.disconnect(), "unable to disconnect from test database: " + e.getMessage());
   RETHROW(connection.connect(environment, getConnectionString()), "unable to connect to benchmark database: " + e.getMessage() + "\n" + getConnectionString());
}
//---------------------------------------------------------------------------
unique_ptr<ConnectionConfig> TiDB::prepareConnectionConfig() const
// Prepare the connection config
{
   return make_unique<odbc::OdbcConnectionConfig>();
}
//---------------------------------------------------------------------------
std::unique_ptr<DatabaseDriver> TiDB::prepareDriver(const string& driverConfigFile, const std::string& logfile)
// Prepare teh database driver
{
   auto driverConfig = make_unique<DriverConfig>();
   if (auto res = driverConfig->read(driverConfigFile); !res)
      throw Exception("unable to read driver config at \"" + driverConfigFile + "\": " + res.getMessage());

   switch (benchmark.get()) {
      case Benchmark::TPCH: return make_unique<TpchDriver>(*this, environment, std::move(driverConfig), std::move(logfile));
      case Benchmark::TPCH1000: return make_unique<TpchDriver>(*this, environment, std::move(driverConfig), std::move(logfile));
      case Benchmark::HATTRICK: return make_unique<HattrickDriver>(*this, environment, std::move(driverConfig), std::move(logfile));
      default: throw Exception("benchmark \"" + benchmark.toString() + "\" not supported");
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
