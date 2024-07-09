#include "singlestore/SingleStoreDriver.hpp"
#include "singlestore/SingleStore.hpp"
#include "singlestore/SingleStoreClient.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/Connection.hpp"
#include <iostream>
#include <thread>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
SingleStoreDriver::SingleStoreDriver(Database& database, const odbc::Environment& environment, unique_ptr<DriverConfig> driverConfig, string logfile)
   : DatabaseDriver(database, std::move(driverConfig), std::move(logfile)), environment(environment)
// Constructor
{
}
//---------------------------------------------------------------------------
SingleStoreDriver::~SingleStoreDriver()
// Destructor
{
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
