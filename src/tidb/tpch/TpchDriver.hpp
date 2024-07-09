#pragma once
//---------------------------------------------------------------------------
#include "tidb/TiDBDriver.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tidb {
//---------------------------------------------------------------------------
/// TPCH driver
class TpchDriver : public TiDBDriver {
   protected:
   /// Create a client
   std::unique_ptr<DatabaseClient> createClient(unsigned clientIndex) override;

   /// Monitor the driver
   void monitor() override;

   public:
   /// Constructor
   TpchDriver(Database& database, const odbc::Environment& environment, std::unique_ptr<DriverConfig> driverConfig, std::string logfile);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
