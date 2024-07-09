#pragma once
//---------------------------------------------------------------------------
#include "txbench/tpch/TpchQueryGenerator.hpp"
#include "umbra/UmbraClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// The query client for TPCH
class TpchQueryClient : public UmbraClient {
   /// The query generator
   tpch::TpchQueryGenerator generator;
   /// The query statements
   std::vector<pq::PqPreparedStatement> statements;

   protected:
   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit TpchQueryClient(const Database& database, unsigned clientId);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
