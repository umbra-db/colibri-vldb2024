#include "duckdb/DuckDBDriver.hpp"
#include "duckdb/DuckDB.hpp"
#include "duckdb/DuckDBClient.hpp"
#include "txbench/Exception.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
DuckDBDriver::DuckDBDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : DatabaseDriver(database, std::move(driverConfig), std::move(logfile))
// Constructor
{
}
//---------------------------------------------------------------------------
DuckDBDriver::~DuckDBDriver()
// Destructor
{
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
