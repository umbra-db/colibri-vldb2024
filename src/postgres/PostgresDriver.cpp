#include "postgres/PostgresDriver.hpp"
#include "txbench/Database.hpp"
#include "txbench/Exception.hpp"
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
PostgresDriver::PostgresDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : DatabaseDriver(database, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
PostgresDriver::~PostgresDriver()
// Destructor
{
}
//---------------------------------------------------------------------------
void PostgresDriver::loadData()
// Load the data
{
   DatabaseDriver::loadData();

   database.executeQuery("vacuum analyze");
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
