#include "duckdb/hattrick/HattrickDriver.hpp"
#include "duckdb/DuckDB.hpp"
#include "duckdb/hattrick/HattrickQueryClient.hpp"
#include "duckdb/hattrick/HattrickRefreshClient.hpp"
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
HattrickDriver::HattrickDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : DuckDBDriver(database, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseClient> HattrickDriver::createClient(unsigned clientIndex)
// Create a client
{
   const auto transactionalClients = database.getTransactionalClients();
   if (clientIndex < transactionalClients) {
      return make_unique<HattrickRefreshClient>(database, clientIndex + 1);
   } else {
      return make_unique<HattrickQueryClient>(database, clientIndex + 1);
   }
}
//---------------------------------------------------------------------------
void HattrickDriver::monitor()
// Monitor the driver
{
   monitorTransactionsAndQueries();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
