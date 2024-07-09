#pragma once
//---------------------------------------------------------------------------
#include "singlestore/SingleStoreDriver.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// HATTRICK driver
class HattrickDriver : public SingleStoreDriver {
   protected:
   /// Create a client
   std::unique_ptr<DatabaseClient> createClient(unsigned clientIndex) override;

   /// Monitor the driver
   void monitor() override;

   public:
   /// Constructor
   HattrickDriver(Database& database, const odbc::Environment& environment, std::unique_ptr<DriverConfig> driverConfig, std::string logfile);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
