#pragma once
//---------------------------------------------------------------------------
#include "singlestore/SingleStoreClient.hpp"
#include "txbench/odbc/PreparedStatement.hpp"
#include "txbench/tpcc/TpccTransactionGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// TPCC client
class TpccClient : public SingleStoreClient {
   /// The transaction generator
   tpcc::TpccTransactionGenerator generator;
   /// The transaction numbers
   std::vector<SQLINTEGER> transactionNumbers;
   /// The home warehouse ids
   std::vector<SQLINTEGER> homeWarehouseIds;
   /// The warehouse counts
   std::vector<SQLINTEGER> warehouseCounts;
   /// Whether to generate remote accesses
   std::vector<SQLINTEGER> generateRemoteAccesses;
   /// Whether to generate rollbacks
   std::vector<SQLINTEGER> generateUserRollbacks;

   /// The dispatch statement
   odbc::PreparedStatement dispatch;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit TpccClient(const Database& database, const odbc::Environment& environment, unsigned homeWarehouseId);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
