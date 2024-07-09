#include "umbra/tpcc/TpccDriver.hpp"
#include "umbra/tpcc/TpccClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
TpccDriver::TpccDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : UmbraDriver(database, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseClient> TpccDriver::createClient(unsigned clientIndex)
// Create a client
{
   return make_unique<TpccClient>(database, clientIndex + 1);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
