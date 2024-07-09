#pragma once
//---------------------------------------------------------------------------
#include "txbench/odbc/OdbcConnectionConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// Connection configuration
class SingleStoreConnectionConfig : public odbc::OdbcConnectionConfig {
   public:
   /// Get the connection string
   std::string getConnectionString(const std::string& database) const override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
