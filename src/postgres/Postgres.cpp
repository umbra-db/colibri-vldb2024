#include "postgres/Postgres.hpp"
#include "postgres/PostgresDriver.hpp"
#include "postgres/hattrick/HattrickDriver.hpp"
#include "postgres/hybench/HyBenchDriver.hpp"
#include "postgres/tatp/TatpDriver.hpp"
#include "postgres/tpcc/TpccDriver.hpp"
#include "postgres/tpcch/TpcchDriver.hpp"
#include "postgres/tpch/TpchDriver.hpp"
#include "txbench/DriverConfig.hpp"
#include "txbench/Exception.hpp"
#include "txbench/pq/PqConnectionConfig.hpp"
#include "txbench/util/sql_helper.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
Postgres::Postgres(System database, Benchmark benchmark, filesystem::path scriptsPath, filesystem::path logPath, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients)
   : Database(database, benchmark, move(scriptsPath), move(logPath), scaleFactor, transactionalClients, analyticalClients)
// Constructor
{
}
//---------------------------------------------------------------------------
void Postgres::executeQuery(const string& query)
// Execute a query
{
   std::vector<string> queries;
   splitSQL(query, queries);

   for (auto& q : queries) {
      connection.execute(q);
   }
}
//---------------------------------------------------------------------------
void Postgres::connect()
// Connect to the database
{
   connection.connect(getConnectionString());
}
//---------------------------------------------------------------------------
unique_ptr<ConnectionConfig> Postgres::prepareConnectionConfig() const
// Prepare the connection config
{
   return make_unique<pq::PqConnectionConfig>();
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseDriver> Postgres::prepareDriver(const string& driverConfigFile, const string& logfile)
// Prepare the database driver a benchmark
{
   auto driverConfig = make_unique<DriverConfig>();
   if (auto res = driverConfig->read(driverConfigFile); !res)
      throw Exception("unable to read driver config at \"" + driverConfigFile + "\": " + res.getMessage());

   switch (benchmark.get()) {
      case Benchmark::TPCC: return make_unique<TpccDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::TPCCH: return make_unique<TpcchDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::TATP: return make_unique<TatpDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::TPCH: return make_unique<TpchDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::TPCH1000: return make_unique<TpchDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::HATTRICK: return make_unique<HattrickDriver>(*this, std::move(driverConfig), std::move(logfile));
      case Benchmark::HYBENCH: return make_unique<HyBenchDriver>(*this, std::move(driverConfig), std::move(logfile));
      default: throw Exception("benchmark \"" + benchmark.toString() + "\" not supported");
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
