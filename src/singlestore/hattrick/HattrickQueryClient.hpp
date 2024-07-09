#pragma once
//---------------------------------------------------------------------------
#include "singlestore/SingleStoreClient.hpp"
#include "txbench/hattrick/QueryGenerator.hpp"
#include "txbench/odbc/PreparedStatement.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// HATTRICK client
class HattrickQueryClient : public SingleStoreClient {
   /// The query generator
   hattrick::QueryGenerator generator;
   /// The query statements
   std::vector<odbc::PreparedStatement> statements;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit HattrickQueryClient(const Database& database, const odbc::Environment& environment, unsigned clientIndex);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
