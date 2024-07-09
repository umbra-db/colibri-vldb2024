#include "umbra/tpch/TpchDriver.hpp"
#include "txbench/Database.hpp"
#include "umbra/tpch/TpchQueryClient.hpp"
#include "umbra/tpch/TpchRefreshClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
TpchDriver::TpchDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : UmbraDriver(database, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseClient> TpchDriver::createClient(unsigned clientIndex)
// Create a client
{
   auto transactionalClients = database.getTransactionalClients();
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
