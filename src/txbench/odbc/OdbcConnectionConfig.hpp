#pragma once
//---------------------------------------------------------------------------
#include "txbench/ConnectionConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::odbc {
//---------------------------------------------------------------------------
/// Connection configuration
class OdbcConnectionConfig : public ConnectionConfig {
   /// The driver
   std::string driver;

   /// The client isolation level
   std::string isolationLevel;

   protected:
   /// Read the values
   Status readValues(JsonParser& parser) override;

   public:
   /// Get the connection string
   std::string getConnectionString(const std::string& database) const override;
   /// Get the isolation level
   const std::string& getIsolationLevel() const { return isolationLevel; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
