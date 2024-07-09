#pragma once
//---------------------------------------------------------------------------
#include "singlestore/SingleStoreClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// TPCH client
class TpchRefreshClient : public SingleStoreClient {
   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit TpchRefreshClient(const Database& database, const odbc::Environment& environment);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
