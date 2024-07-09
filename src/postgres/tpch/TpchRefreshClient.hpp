#pragma once
//---------------------------------------------------------------------------
#include "postgres/PostgresClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// The refresh client for TPCH
class TpchRefreshClient : public PostgresClient {
   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit TpchRefreshClient(const Database& database);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
