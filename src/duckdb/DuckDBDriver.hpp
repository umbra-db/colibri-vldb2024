#pragma once
//---------------------------------------------------------------------------
#include "txbench/DatabaseDriver.hpp"
#include <memory>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
namespace duckdb {
//---------------------------------------------------------------------------
/// A generic benchmark driver implementation
class DuckDBDriver : public DatabaseDriver {
   public:
   /// Constructor
   DuckDBDriver(Database& database, std::unique_ptr<DriverConfig> driverConfig, std::string logfile);
   /// Destructor
   ~DuckDBDriver() override;
};
//---------------------------------------------------------------------------
}
}
//---------------------------------------------------------------------------
