#pragma once
//---------------------------------------------------------------------------
#include "duckdb/DuckDBClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
/// TPCH client
class TpchRefreshClient : public DuckDBClient {
   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit TpchRefreshClient(const Database& database);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
