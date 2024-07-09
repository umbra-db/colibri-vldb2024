#include "umbra/hattrick/HattrickDriver.hpp"
#include "HattrickRefreshClient.hpp"
#include "txbench/Database.hpp"
#include "umbra/hattrick/HattrickQueryClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
HattrickDriver::HattrickDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : UmbraDriver(database, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseClient> HattrickDriver::createClient(unsigned clientIndex)
// Create a client
{
   auto transactionalClients = database.getTransactionalClients();
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
