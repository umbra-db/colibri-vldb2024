#include "umbra/tatp/TatpDriver.hpp"
#include "umbra/tatp/TatpClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
TatpDriver::TatpDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : UmbraDriver(database, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseClient> TatpDriver::createClient(unsigned clientIndex)
// Create a client
{
   return make_unique<TatpClient>(database, clientIndex + 1);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
