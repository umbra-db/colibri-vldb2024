#pragma once
//---------------------------------------------------------------------------
#include "txbench/hybench/Generator.hpp"
#include "txbench/hybench/TransactionGenerator.hpp"
#include "txbench/pq/PqPreparedStatement.hpp"
#include "postgres/PostgresClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
struct HyBenchSharedState;
//---------------------------------------------------------------------------
/// The refresh client for HATTRICK
class HyBenchRefreshClient : public PostgresClient {
   /// The config
   hybench::Config config;
   /// The generator
   hybench::Generator generator;
   /// The transaction generator
   hybench::TransactionGenerator transactionGenerator;

   /// The prepared statements
   std::array<pq::PqPreparedStatement, 6> statements;

   /// The prepared statement to retrieve the blocked accounts
   pq::PqPreparedStatement retrieveBlockedAccounts;
   /// The prepared statement to drop the blocked accounts
   pq::PqPreparedStatement dropBlockedAccounts;

   /// The shared state
   HyBenchSharedState& state;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit HyBenchRefreshClient(const Database& database, HyBenchSharedState& state, unsigned clientId);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
