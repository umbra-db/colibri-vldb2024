#pragma once
//---------------------------------------------------------------------------
#include "postgres/PostgresDriver.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// TPCC driver
class TpccDriver : public PostgresDriver {
   protected:
   /// Create a client
   std::unique_ptr<DatabaseClient> createClient(unsigned clientIndex) override;

   public:
   /// Constructor
   TpccDriver(Database& database, std::unique_ptr<DriverConfig> tpccConfig, std::string logfile);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
