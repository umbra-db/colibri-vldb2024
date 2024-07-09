#pragma once
//---------------------------------------------------------------------------
#include "singlestore/SingleStoreDriver.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// TPCC driver
class TpccDriver : public SingleStoreDriver {
   protected:
   /// Create a client
   std::unique_ptr<DatabaseClient> createClient(unsigned clientIndex) override;

   public:
   /// Constructor
   TpccDriver(Database& database, const odbc::Environment& environment, std::unique_ptr<DriverConfig> tpccConfig, std::string logfile);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
