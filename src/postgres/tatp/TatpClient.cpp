#include "postgres/tatp/TatpClient.hpp"
#include "txbench/Database.hpp"
#include "txbench/Exception.hpp"
#include "txbench/pq/PqResult.hpp"
#include <charconv>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
TatpClient::TatpClient(const Database& database, unsigned clientIndex)
   : PostgresClient(database), generator(42 + clientIndex), subscriberCount(0)
// Constructor
{
}
//---------------------------------------------------------------------------
void TatpClient::performWork()
// Perform work
{
   auto pipelineDepth = database.getPipelineDepth();
   connection.enterPipelineMode();

   auto pickStatement = [&]() -> pq::PqPreparedStatement& {
      switch (generator.pickTransactionType()) {
         case tatp::TatpTransactionGenerator::TransactionType::GetSubscriberData: return getSubscriberData;
         case tatp::TatpTransactionGenerator::TransactionType::GetNewDestination: return getNewDestination;
         case tatp::TatpTransactionGenerator::TransactionType::GetAccessData: return getAccessData;
         case tatp::TatpTransactionGenerator::TransactionType::UpdateSubscriberData: return updateSubscriberData;
         case tatp::TatpTransactionGenerator::TransactionType::UpdateLocation: return updateLocation;
         case tatp::TatpTransactionGenerator::TransactionType::InsertCallForwarding: return insertCallForwarding;
         case tatp::TatpTransactionGenerator::TransactionType::DeleteCallForwarding: return deleteCallForwarding;
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
void TatpClient::initialize()
// Initialize the client
{
   PostgresClient::initialize();

   {
      // Retrieve the number of subscribers
      auto result = connection.execute("SELECT COUNT(*) FROM subscriber");

      auto field = *result.getvalue(0, 0);
      auto [ptr, ec] = from_chars(field.data(), field.data() + field.size(), subscriberCount);
      if (ptr != (field.data() + field.size()))
         throw Exception("unable to parse subscriber count '" + string(field) + "'");
   }

   getSubscriberData = connection.prepare("getSubscriberData", "CALL get_subscriber_data($1::INTEGER)");
   getSubscriberData.bind(subscriberCount);

   getNewDestination = connection.prepare("getNewDestination", "CALL get_new_destination($1::INTEGER)");
   getNewDestination.bind(subscriberCount);

   getAccessData = connection.prepare("getAccessData", "CALL get_access_data($1::INTEGER)");
   getAccessData.bind(subscriberCount);

   updateSubscriberData = connection.prepare("updateSubscriberData", "CALL update_subscriber_data($1::INTEGER)");
   updateSubscriberData.bind(subscriberCount);

   updateLocation = connection.prepare("updateLocation", "CALL update_location($1::INTEGER)");
   updateLocation.bind(subscriberCount);

   insertCallForwarding = connection.prepare("insertCallForwarding", "CALL insert_call_forwarding($1::INTEGER)");
   insertCallForwarding.bind(subscriberCount);

   deleteCallForwarding = connection.prepare("deleteCallForwarding", "CALL delete_call_forwarding($1::INTEGER)");
   deleteCallForwarding.bind(subscriberCount);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
