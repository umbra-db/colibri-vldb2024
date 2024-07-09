#pragma once
//---------------------------------------------------------------------------
#include "postgres/PostgresClient.hpp"
#include "txbench/pq/PqPreparedStatement.hpp"
#include "txbench/tpch/TpchQueryGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// The query client for TPCH
class TpchQueryClient : public PostgresClient {
   /// The query generator
   tpch::TpchQueryGenerator generator;
   /// The query statements
   std::vector<pq::PqPreparedStatement> statements;

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
