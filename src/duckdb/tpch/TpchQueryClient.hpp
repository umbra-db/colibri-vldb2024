#pragma once
//---------------------------------------------------------------------------
#include "duckdb/DuckDBClient.hpp"
#include "txbench/tpch/TpchQueryGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
/// TPCH client
class TpchQueryClient : public DuckDBClient {
   /// The query generator
   tpch::TpchQueryGenerator generator;
   /// The query statements
   std::vector<DuckDBStatement> statements;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit TpchQueryClient(const Database& database, unsigned clientIndex);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
