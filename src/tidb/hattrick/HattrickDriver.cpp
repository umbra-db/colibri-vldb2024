#include "tidb/hattrick/HattrickDriver.hpp"
#include "tidb/TiDB.hpp"
#include "tidb/hattrick/HattrickQueryClient.hpp"
#include "tidb/hattrick/HattrickRefreshClient.hpp"
#include "txbench/DriverConfig.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/Connection.hpp"
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::tidb {
//---------------------------------------------------------------------------
HattrickDriver::HattrickDriver(Database& database, const odbc::Environment& environment, unique_ptr<DriverConfig> driverConfig, string logfile)
   : TiDBDriver(database, environment, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseClient> HattrickDriver::createClient(unsigned clientIndex)
// Create a client
{
   const auto transactionalClients = database.getTransactionalClients();
   if (clientIndex < transactionalClients) {
      return make_unique<HattrickRefreshClient>(database, environment, clientIndex + 1);
   } else {
      return make_unique<HattrickQueryClient>(database, environment, clientIndex + 1);
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
