#pragma once
//---------------------------------------------------------------------------
#include "txbench/DatabaseDriver.hpp"
#include <memory>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
namespace odbc {
//---------------------------------------------------------------------------
class Environment;
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
namespace tidb {
//---------------------------------------------------------------------------
/// A generic benchmark driver implementation
class TiDBDriver : public DatabaseDriver {
   protected:
   /// The environment
   const odbc::Environment& environment;

   public:
   /// Constructor
   TiDBDriver(Database& database, const odbc::Environment& environment, std::unique_ptr<DriverConfig> driverConfig, std::string logfile);
   /// Destructor
   ~TiDBDriver() override;
};
//---------------------------------------------------------------------------
}
}
//---------------------------------------------------------------------------
