#pragma once
//---------------------------------------------------------------------------
#include "umbra/UmbraClient.hpp"
#include <thread>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// The refresh client for TPCH
class TpchRefreshClient : public UmbraClient {
   protected:
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
