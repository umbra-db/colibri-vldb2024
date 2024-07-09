#pragma once
//---------------------------------------------------------------------------
#include "postgres/PostgresClient.hpp"
#include "txbench/hattrick/QueryGenerator.hpp"
#include "txbench/pq/PqPreparedStatement.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// The query client for HATTRICK
class HattrickQueryClient : public PostgresClient {
   /// The query generator
   hattrick::QueryGenerator generator;
   /// The query statements
   std::vector<pq::PqPreparedStatement> statements;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit HattrickQueryClient(const Database& database, unsigned clientId);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
