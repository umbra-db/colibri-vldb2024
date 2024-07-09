#include "umbra/tpcc/TpccClient.hpp"
#include "txbench/Database.hpp"
#include "txbench/Exception.hpp"
#include "txbench/tpcc/TpccConfig.hpp"
#include <charconv>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
TpccClient::TpccClient(const Database& database, unsigned homeWarehouseId)
   : UmbraClient(database), generator(42 + homeWarehouseId), homeWarehouseId(homeWarehouseId), warehouseCount(0)
// Constructor
{
}
//---------------------------------------------------------------------------
void TpccClient::performWork()
// Perform work
{
   auto pipelineDepth = database.getPipelineDepth();

   connection.enterPipelineMode();

   auto pickStatement = [&]() -> pq::PqPreparedStatement& {
      switch (generator.pickTransactionType()) {
         case tpcc::TpccTransactionGenerator::TransactionType::Delivery: return delivery;
         case tpcc::TpccTransactionGenerator::TransactionType::NewOrder: return newOrder;
         case tpcc::TpccTransactionGenerator::TransactionType::OrderStatus: return orderStatus;
         case tpcc::TpccTransactionGenerator::TransactionType::Payment: return payment;
         case tpcc::TpccTransactionGenerator::TransactionType::StockLevel: return stockLevel;
      }
      __builtin_unreachable();
   };
   auto sendBatch = [&]() {
      // Send a batch of transactions
      for (unsigned i = 0; i < pipelineDepth; ++i) {
         connection.send(pickStatement());
         submittedTransactions.fetch_add(1, memory_order_acq_rel);
      }

      // Request that the server flushes results
      connection.sendFlushRequest();

      // And submit to the server
      connection.flush();
   };
   auto receiveBatch = [&]() {
      // Receive a batch of transactions
      for (unsigned i = 0; i < pipelineDepth; ++i) {
         connection.receive();
         processedTransactions.fetch_add(1, memory_order_acq_rel);
      }
   };

   sendBatch();
   while (running()) {
      sendBatch();
      receiveBatch();
   }

   receiveBatch();

   connection.sync();
   connection.exitPipelineMode();
}
//---------------------------------------------------------------------------
void TpccClient::initialize()
// Initialize the client
{
   UmbraClient::initialize();

   connection.execute("CREATE TEMPORARY TABLE positions\n"
                      "(\n"
                      "    ol_i_id        INTEGER NOT NULL,\n"
                      "    ol_number      INTEGER NOT NULL,\n"
                      "    ol_supply_w_id INTEGER NOT NULL,\n"
                      "    ol_quantity    INTEGER NOT NULL\n"
                      ") ON COMMIT DELETE ROWS;");

   {
      // Retrieve the number of warehouses
      auto result = connection.execute("SELECT COUNT(*) FROM warehouse");

      auto field = *result.getvalue(0, 0);
      auto [ptr, ec] = from_chars(field.data(), field.data() + field.size(), warehouseCount);
      if (ptr != (field.data() + field.size()))
         throw Exception("unable to parse warehouse count '" + string(field) + "'");
      if (homeWarehouseId > warehouseCount)
         throw Exception("invalid home warehouse id " + to_string(homeWarehouseId));
   }

   const auto& tpccConfig = reinterpret_cast<const tpcc::TpccConfig&>(database.getBenchmarkConfig());
   bool generateRemoteAccesses = tpccConfig.shouldGenerateRemoteAccesses();
   bool generateUserRollbacks = tpccConfig.shouldGenerateUserRollbacks();

   delivery = connection.prepare("delivery", "CALL delivery($1::INTEGER)");
   delivery.bind(homeWarehouseId);

   newOrder = connection.prepare("newOrder", "CALL newOrder($1::INTEGER, $2::INTEGER, $3::INTEGER, $4::INTEGER)");
   newOrder.bind(homeWarehouseId, warehouseCount, generateRemoteAccesses, generateUserRollbacks);

   orderStatus = connection.prepare("orderStatus", "CALL orderStatus($1::INTEGER)");
   orderStatus.bind(homeWarehouseId);

   payment = connection.prepare("payment", "CALL payment($1::INTEGER, $2::INTEGER, $3::INTEGER)");
   payment.bind(homeWarehouseId, warehouseCount, generateRemoteAccesses);

   stockLevel = connection.prepare("stockLevel", "CALL stockLevel($1::INTEGER)");
   stockLevel.bind(homeWarehouseId);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
