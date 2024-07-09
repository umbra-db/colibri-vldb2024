#pragma once
//---------------------------------------------------------------------------
#include "tidb/TiDBClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tidb {
//---------------------------------------------------------------------------
/// TPCH client
class TpchRefreshClient : public TiDBClient {
   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit TpchRefreshClient(const Database& database, const odbc::Environment& environment);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
