#pragma once
//---------------------------------------------------------------------------
#include "duckdb/DuckDBClient.hpp"
#include "txbench/hattrick/QueryGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
/// HATTRICK client
class HattrickQueryClient : public DuckDBClient {
   /// The query generator
   hattrick::QueryGenerator generator;
   /// The query statements
   std::vector<DuckDBStatement> statements;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit HattrickQueryClient(const Database& database, unsigned clientIndex);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
