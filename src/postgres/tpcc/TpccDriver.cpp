#include "postgres/tpcc/TpccDriver.hpp"
#include "postgres/tpcc/TpccClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
TpccDriver::TpccDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : PostgresDriver(database, std::move(driverConfig), std::move(logfile))
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
