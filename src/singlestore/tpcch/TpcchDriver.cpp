#include "singlestore/tpcch/TpcchDriver.hpp"
#include "singlestore/SingleStore.hpp"
#include "singlestore/tpcc/TpccClient.hpp"
#include "singlestore/tpcch/TpcchQueryClient.hpp"
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
TpcchDriver::TpcchDriver(Database& database, const odbc::Environment& environment, unique_ptr<DriverConfig> driverConfig, string logfile)
   : SingleStoreDriver(database, environment, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseClient> TpcchDriver::createClient(unsigned clientIndex)
// Create a client
{
   const auto transactionalClients = database.getTransactionalClients();
   if (clientIndex < transactionalClients) {
      return make_unique<TpccClient>(database, environment, clientIndex + 1);
   } else {
      return make_unique<TpcchQueryClient>(database, environment, clientIndex + 1);
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
