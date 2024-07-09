#pragma once
//---------------------------------------------------------------------------
#include "umbra/UmbraDriver.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// TPCCH driver
class TpcchDriver : public UmbraDriver {
   protected:
   /// Create a client
   std::unique_ptr<DatabaseClient> createClient(unsigned clientIndex) override;

   /// Monitor the driver
   void monitor() override;

   public:
   /// Constructor
   TpcchDriver(Database& database, std::unique_ptr<DriverConfig> driverConfig, std::string logfile);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
