#pragma once
//---------------------------------------------------------------------------
#include "txbench/DatabaseDriver.hpp"
#include <memory>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// A postgres benchmark driver implementation
class PostgresDriver : public DatabaseDriver {
   protected:
   /// Load the data
   void loadData() override;

   public:
   /// Constructor
   PostgresDriver(Database& database, std::unique_ptr<DriverConfig> driverConfig, std::string logfile);
   /// Destructor
   ~PostgresDriver() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
