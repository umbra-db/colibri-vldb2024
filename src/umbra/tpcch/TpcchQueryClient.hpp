#pragma once
//---------------------------------------------------------------------------
#include "txbench/tpch/TpchQueryGenerator.hpp"
#include "umbra/UmbraClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// The query client for TPCCH
class TpcchQueryClient : public UmbraClient {
   /// The query generator
   tpch::TpchQueryGenerator generator;
   /// The query statements
   std::vector<pq::PqPreparedStatement> statements;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit TpcchQueryClient(const Database& database, unsigned clientId);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
