#pragma once
//---------------------------------------------------------------------------
#include "singlestore/SingleStoreClient.hpp"
#include "txbench/odbc/PreparedStatement.hpp"
#include "txbench/tatp/TatpTransactionGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// TATP client
class TatpClient : public SingleStoreClient {
   /// The transaction generator
   tatp::TatpTransactionGenerator generator;
   /// The transaction numbers
   std::vector<SQLINTEGER> transactionNumbers;
   /// The subscriber counts
   std::vector<SQLINTEGER> subscriberCounts;

   /// The dispatch statement
   odbc::PreparedStatement dispatch;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit TatpClient(const Database& database, const odbc::Environment& environment, unsigned clientIndex);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
