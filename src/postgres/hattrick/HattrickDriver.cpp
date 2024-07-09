#include "postgres/hattrick/HattrickDriver.hpp"
#include "postgres/hattrick/HattrickQueryClient.hpp"
#include "postgres/hattrick/HattrickRefreshClient.hpp"
#include "txbench/Database.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
HattrickDriver::HattrickDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : PostgresDriver(database, std::move(driverConfig), std::move(logfile))
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
