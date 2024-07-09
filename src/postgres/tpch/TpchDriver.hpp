#pragma once
//---------------------------------------------------------------------------
#include "postgres/PostgresDriver.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// TPCH driver
class TpchDriver : public PostgresDriver {
   protected:
   /// Create a client
   std::unique_ptr<DatabaseClient> createClient(unsigned clientIndex) override;

   /// Monitor the driver
   void monitor() override;

   public:
   /// Constructor
   TpchDriver(Database& database, std::unique_ptr<DriverConfig> driverConfig, std::string logfile);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
