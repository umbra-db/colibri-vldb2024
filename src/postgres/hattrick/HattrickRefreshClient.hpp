#pragma once
//---------------------------------------------------------------------------
#include "postgres/PostgresClient.hpp"
#include "txbench/hattrick/Generator.hpp"
#include "txbench/hattrick/TransactionGenerator.hpp"
#include "txbench/pq/PqPreparedStatement.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// The refresh client for HATTRICK
class HattrickRefreshClient : public PostgresClient {
   /// The transaction generator
   hattrick::TransactionGenerator transactionGenerator;
   /// The data generator
   hattrick::Generator generator;

   /// The new order statement
   pq::PqPreparedStatement newOrder;
   /// The payment statement
   pq::PqPreparedStatement payment;
   /// The count orders statement
   pq::PqPreparedStatement countOrders;

   /// The number of customers
   uint64_t customerCount;
   /// The number of parts
   uint64_t partCount;
   /// The number of suppliers
   uint64_t supplierCount;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit HattrickRefreshClient(const Database& database, unsigned clientId);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
