#include "tidb/tpch/TpchDriver.hpp"
#include "tidb/TiDB.hpp"
#include "tidb/tpch/TpchQueryClient.hpp"
#include "tidb/tpch/TpchRefreshClient.hpp"
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
TpchDriver::TpchDriver(Database& database, const odbc::Environment& environment, unique_ptr<DriverConfig> driverConfig, string logfile)
   : TiDBDriver(database, environment, std::move(driverConfig), std::move(logfile))
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
