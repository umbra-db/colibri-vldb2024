#pragma once
//---------------------------------------------------------------------------
#include "txbench/ConnectionConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::pq {
//---------------------------------------------------------------------------
/// Client configuration
class PqConnectionConfig : public ConnectionConfig {
   public:
   /// Get the connection string
   std::string getConnectionString(const std::string& database) const override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
