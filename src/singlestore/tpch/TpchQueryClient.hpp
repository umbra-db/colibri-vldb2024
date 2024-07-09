#pragma once
//---------------------------------------------------------------------------
#include "singlestore/SingleStoreClient.hpp"
#include "txbench/odbc/PreparedStatement.hpp"
#include "txbench/tpch/TpchQueryGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// TPCH client
class TpchQueryClient : public SingleStoreClient {
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
