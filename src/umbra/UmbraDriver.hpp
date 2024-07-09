#pragma once
//---------------------------------------------------------------------------
#include "txbench/DatabaseDriver.hpp"
#include <memory>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// The umbra benchmark driver
class UmbraDriver : public DatabaseDriver {
   protected:
   /// Create the schema
   void createSchema() override;

   public:
   /// Constructor
   UmbraDriver(Database& database, std::unique_ptr<DriverConfig> driverConfig, std::string logfile);
   /// Destructor
   ~UmbraDriver() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
