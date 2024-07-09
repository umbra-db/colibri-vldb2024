#pragma once
//---------------------------------------------------------------------------
#include "txbench/hattrick/QueryGenerator.hpp"
#include "txbench/pq/PqPreparedStatement.hpp"
#include "umbra/UmbraClient.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// The query client for HATTRICK
class HattrickQueryClient : public UmbraClient {
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
