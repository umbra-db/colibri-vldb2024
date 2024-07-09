#pragma once
//---------------------------------------------------------------------------
#include "txbench/hybench/Generator.hpp"
#include "txbench/hybench/TransactionGenerator.hpp"
#include "txbench/pq/PqPreparedStatement.hpp"
#include "umbra/UmbraClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
struct HyBenchSharedState;
//---------------------------------------------------------------------------
/// The refresh client for HATTRICK
class HyBenchOLTPClient : public UmbraClient {
   /// The config
   hybench::Config config;
   /// The generator
   hybench::Generator generator;
   /// The transaction generator
   hybench::TransactionGenerator transactionGenerator;

   /// The prepared statements
   std::array<pq::PqPreparedStatement, 18> statements;

   /// The max company size
   uint64_t maxCompanySize = 0;
   /// The shared state
   HyBenchSharedState& state;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit HyBenchOLTPClient(const Database& database, HyBenchSharedState& state, unsigned clientId);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
