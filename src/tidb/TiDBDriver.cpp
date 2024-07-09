#include "tidb/TiDBDriver.hpp"
#include "tidb/TiDB.hpp"
#include "tidb/TiDBClient.hpp"
#include "txbench/Exception.hpp"
#include <thread>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::tidb {
//---------------------------------------------------------------------------
TiDBDriver::TiDBDriver(Database& database, const odbc::Environment& environment, unique_ptr<DriverConfig> driverConfig, string logfile)
   : DatabaseDriver(database, std::move(driverConfig), std::move(logfile)), environment(environment)
// Constructor
{
}
//---------------------------------------------------------------------------
TiDBDriver::~TiDBDriver()
// Destructor
{
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
