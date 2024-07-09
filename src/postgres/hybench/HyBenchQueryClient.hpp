#pragma once
//---------------------------------------------------------------------------
#include "txbench/hybench/Generator.hpp"
#include "txbench/hybench/QueryGenerator.hpp"
#include "postgres/PostgresClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
struct HyBenchSharedState;
//---------------------------------------------------------------------------
/// The olxp query client for TPCH
class HyBenchQueryClient : public PostgresClient {
   /// The config
   hybench::Config config;
   /// The generator
   hybench::Generator generator;
   /// The query generator
   hybench::QueryGenerator queryGenerator;
   /// The query statements
   std::vector<pq::PqPreparedStatement> statements;

   /// The shared state
   HyBenchSharedState& state;

   protected:
   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit HyBenchQueryClient(const Database& database, HyBenchSharedState& state, unsigned clientId);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
