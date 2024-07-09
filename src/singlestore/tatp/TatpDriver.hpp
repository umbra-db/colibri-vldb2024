#pragma once
//---------------------------------------------------------------------------
#include "singlestore/SingleStoreDriver.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// TATP driver
class TatpDriver : public SingleStoreDriver {
   protected:
   /// Create a client
   std::unique_ptr<DatabaseClient> createClient(unsigned clientIndex) override;

   public:
   /// Constructor
   TatpDriver(Database& database, const odbc::Environment& environment, std::unique_ptr<DriverConfig> driverConfig, std::string logfile);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
