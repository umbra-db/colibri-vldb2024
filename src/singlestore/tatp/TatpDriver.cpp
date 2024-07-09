#include "singlestore/tatp/TatpDriver.hpp"
#include "singlestore/SingleStore.hpp"
#include "singlestore/tatp/TatpClient.hpp"
#include "txbench/DriverConfig.hpp"
#include "txbench/Exception.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
TatpDriver::TatpDriver(Database& database, const odbc::Environment& environment, unique_ptr<DriverConfig> driverConfig, string logfile)
   : SingleStoreDriver(database, environment, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseClient> TatpDriver::createClient(unsigned clientIndex)
// Create a client
{
   return make_unique<TatpClient>(database, environment, clientIndex + 1);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
