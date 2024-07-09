#include "singlestore/tpch/TpchDriver.hpp"
#include "singlestore/SingleStore.hpp"
#include "singlestore/tpch/TpchQueryClient.hpp"
#include "singlestore/tpch/TpchRefreshClient.hpp"
#include "txbench/DriverConfig.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/Connection.hpp"
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
TpchDriver::TpchDriver(Database& database, const odbc::Environment& environment, unique_ptr<DriverConfig> driverConfig, string logfile)
   : SingleStoreDriver(database, environment, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseClient> TpchDriver::createClient(unsigned clientIndex)
// Create a client
{
   const auto transactionalClients = database.getTransactionalClients();
   if (clientIndex < transactionalClients) {
      return make_unique<TpchRefreshClient>(database, environment);
   } else {
      return make_unique<TpchQueryClient>(database, environment, clientIndex + 1);
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
