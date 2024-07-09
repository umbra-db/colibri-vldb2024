#pragma once
//---------------------------------------------------------------------------
#include "postgres/PostgresClient.hpp"
#include "txbench/pq/PqPreparedStatement.hpp"
#include "txbench/tpcc/TpccTransactionGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// TPCC client
class TpccClient : public PostgresClient {
   /// The transaction generator
   tpcc::TpccTransactionGenerator generator;
   /// The home warehouse id
   unsigned homeWarehouseId;
   /// The warehouse count
   unsigned warehouseCount;

   /// The delivery statement
   pq::PqPreparedStatement delivery;
   /// The new order statement
   pq::PqPreparedStatement newOrder;
   /// The order status statement
   pq::PqPreparedStatement orderStatus;
   /// The payment statement
   pq::PqPreparedStatement payment;
   /// The stock level statement
   pq::PqPreparedStatement stockLevel;

   /// Perform work
   void performWork() override;

   public:
   /// Constructor
   explicit TpccClient(const Database& database, unsigned homeWarehouseId);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
