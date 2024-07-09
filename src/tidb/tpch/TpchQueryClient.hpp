#pragma once
//---------------------------------------------------------------------------
#include "tidb/TiDBClient.hpp"
#include "txbench/odbc/PreparedStatement.hpp"
#include "txbench/tpch/TpchQueryGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tidb {
//---------------------------------------------------------------------------
/// TPCH client
class TpchQueryClient : public TiDBClient {
   /// The query generator
   tpch::TpchQueryGenerator generator;
   /// The query statements
   std::vector<odbc::PreparedStatement> statements;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit TpchQueryClient(const Database& database, const odbc::Environment& environment, unsigned clientIndex);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
