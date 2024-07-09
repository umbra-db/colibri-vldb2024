#include "duckdb/tpch/TpchDriver.hpp"
#include "duckdb/DuckDB.hpp"
#include "duckdb/tpch/TpchQueryClient.hpp"
#include "duckdb/tpch/TpchRefreshClient.hpp"
#include "txbench/DriverConfig.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/Connection.hpp"
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
TpchDriver::TpchDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : DuckDBDriver(database, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseClient> TpchDriver::createClient(unsigned clientIndex)
// Create a client
{
   const auto transactionalClients = database.getTransactionalClients();
   if (clientIndex < transactionalClients) {
      return make_unique<TpchRefreshClient>(database);
   } else {
      return make_unique<TpchQueryClient>(database, clientIndex + 1);
   }
}
//---------------------------------------------------------------------------
void TpchDriver::monitor()
// Monitor the driver
{
   monitorTransactionsAndQueries();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
