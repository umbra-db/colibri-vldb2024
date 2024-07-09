#pragma once
//---------------------------------------------------------------------------
#include "txbench/ConnectionConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
/// Connection configuration
class DuckDBConnectionConfig : public ConnectionConfig {
   public:
   /// Get the connection string
   std::string getConnectionString(const std::string& database) const {return "";};
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
