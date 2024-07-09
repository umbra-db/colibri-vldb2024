#include "postgres/tpch/TpchDriver.hpp"
#include "postgres/tpch/TpchQueryClient.hpp"
#include "postgres/tpch/TpchRefreshClient.hpp"
#include "txbench/Database.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
TpchDriver::TpchDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : PostgresDriver(database, std::move(driverConfig), std::move(logfile))
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
