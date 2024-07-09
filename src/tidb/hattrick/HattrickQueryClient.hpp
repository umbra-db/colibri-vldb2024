#pragma once
//---------------------------------------------------------------------------
#include "tidb/TiDBClient.hpp"
#include "txbench/hattrick/QueryGenerator.hpp"
#include "txbench/odbc/PreparedStatement.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tidb {
//---------------------------------------------------------------------------
/// HATTRICK client
class HattrickQueryClient : public TiDBClient {
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
