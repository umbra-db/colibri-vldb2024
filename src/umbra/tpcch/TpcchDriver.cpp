#include "umbra/tpcch/TpcchDriver.hpp"
#include "txbench/Database.hpp"
#include "umbra/tpcc/TpccClient.hpp"
#include "umbra/tpcch/TpcchQueryClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
TpcchDriver::TpcchDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : UmbraDriver(database, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseClient> TpcchDriver::createClient(unsigned clientIndex)
// Create a client
{
   auto transactionalClients = database.getTransactionalClients();
   if (clientIndex < transactionalClients) {
      return make_unique<TpccClient>(database, clientIndex + 1);
   } else {
      return make_unique<TpcchQueryClient>(database, clientIndex + 1);
   }
}
//---------------------------------------------------------------------------
void TpcchDriver::monitor()
// Monitor the driver
{
   monitorTransactionsAndQueries();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
