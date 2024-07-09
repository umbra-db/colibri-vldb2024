#include "postgres/hattrick/HattrickRefreshClient.hpp"
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
/// The next order key
static std::atomic<int> nextOrderKey;
//---------------------------------------------------------------------------
HattrickRefreshClient::HattrickRefreshClient(const Database& database, unsigned clientId)
   : PostgresClient(database), transactionGenerator(42 + clientId), generator(42 + clientId)
// Constructor
{
}
//---------------------------------------------------------------------------
void HattrickRefreshClient::performWork()
// Perform work
{
   auto pipelineDepth = database.getPipelineDepth();
   connection.enterPipelineMode();

   optional<int> lastOrder;

   auto sendBatch = [&]() {
      // Send a batch of transactions
      for (unsigned i = 0; i < pipelineDepth; ++i) {
         switch (transactionGenerator.pickTransactionType()) {
            case hattrick::TransactionGenerator::TransactionType::NewOrderAndPayment: {
               if (lastOrder.has_value()) {
                  int custkey = generator.generateRand<int>(1, customerCount);
                  int suppkey = generator.generateRand<int>(1, supplierCount);

                  string amount = to_string(generator.generateRand(100, 10'495'000) / 100.00);

                  payment.bind(custkey, suppkey, amount, lastOrder.value());
                  connection.send(payment);
                  submittedTransactions.fetch_add(1, memory_order_acq_rel);

                  lastOrder.reset();
               } else {
                  int orderkey = nextOrderKey.fetch_add(1);
                  int custkey = generator.generateRand<int>(1, customerCount);
                  string customerName = "Customer#" + generator.generateDigitString(custkey, 9);

                  int partkey1 = generator.generateRand<int>(1, partCount);
                  int partkey2 = generator.generateRand<int>(1, partCount);
                  int partkey3 = generator.generateRand<int>(1, partCount);
                  int partkey4 = generator.generateRand<int>(1, partCount);

                  int suppkey1 = generator.generateRand<int>(1, supplierCount);
                  int suppkey2 = generator.generateRand<int>(1, supplierCount);
                  int suppkey3 = generator.generateRand<int>(1, supplierCount);
                  int suppkey4 = generator.generateRand<int>(1, supplierCount);
                  string supplierName1 = "Supplier#" + generator.generateDigitString(suppkey1, 9);
                  string supplierName2 = "Supplier#" + generator.generateDigitString(suppkey2, 9);
                  string supplierName3 = "Supplier#" + generator.generateDigitString(suppkey3, 9);
                  string supplierName4 = "Supplier#" + generator.generateDigitString(suppkey4, 9);

                  auto generateDate = [this]() {
                     struct Month {
                        string name;
                        unsigned days;
                     };
                     static const vector<string> years = {"1992", "1993", "1994", "1995", "1996", "1997", "1998"};
                     static const vector<Month> months = {{"January", 31}, {"February", 28}, {"March", 31}, {"April", 30}, {"May", 31}, {"June", 30}, {"July", 31}, {"August", 31}, {"September", 30}, {"October", 31}, {"November", 30}, {"December", 31}};

                     int year = generator.generateRand<int>(1, years.size());
                     int month = generator.generateRand<int>(1, months.size());
                     int day = generator.generateRand<int>(1, months[month - 1].days);
                     return months[month - 1].name + " " + to_string(day) + ", " + years[year - 1];
                  };
                  string date1 = generateDate();
                  string date2 = generateDate();
                  string date3 = generateDate();
                  string date4 = generateDate();

                  // Create the other data of the current lineorder randomly.
                  string ordpriority = generator.generateOrderPriority();
                  string shippriority = to_string(generator.generateRand(0, 1));
                  int quantity = generator.generateRand(1, 50);
                  string extendedprice = to_string(quantity);
                  int discount = generator.generateRand(0, 10);
                  string revenue = to_string((static_cast<double>(quantity) * (100 - discount)) / 100.00);
                  string supplycost = to_string(generator.generateRand(100, 100000) / 100.00);
                  int tax = generator.generateRand(0, 8);
                  string shipmode = generator.generateShipMode();

                  newOrder.bind(orderkey, customerName, partkey1, partkey2, partkey3, partkey4, supplierName1, supplierName2, supplierName3, supplierName4, date1, date2, date3, date4,
                                ordpriority, shippriority, quantity, extendedprice, discount, revenue, supplycost, tax, shipmode);
                  connection.send(newOrder);
                  submittedTransactions.fetch_add(1, memory_order_acq_rel);

                  lastOrder.emplace(orderkey);
               }
            } break;
            case hattrick::TransactionGenerator::TransactionType::CountOrders: {
               int custkey = generator.generateRand<int>(1, customerCount);
               string customerName = "Customer#" + generator.generateDigitString(custkey, 9);
               countOrders.bind(customerName);
               connection.send(countOrders);
               submittedTransactions.fetch_add(1, memory_order_acq_rel);
            } break;
         }
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
void HattrickRefreshClient::initialize()
// Initialize the client
{
   PostgresClient::initialize();

   {
      // Retrieve the number of customer
      auto result = connection.execute("SELECT COUNT(*) FROM customer");

      auto field = *result.getvalue(0, 0);
      auto [ptr, ec] = from_chars(field.data(), field.data() + field.size(), customerCount);
      if (ptr != (field.data() + field.size()))
         throw Exception("unable to parse customer count '" + string(field) + "'");
   }
   {
      // Retrieve the number of parts
      auto result = connection.execute("SELECT COUNT(*) FROM part");

      auto field = *result.getvalue(0, 0);
      auto [ptr, ec] = from_chars(field.data(), field.data() + field.size(), partCount);
      if (ptr != (field.data() + field.size()))
         throw Exception("unable to parse customer part '" + string(field) + "'");
   }
   {
      // Retrieve the number of suppliers
      auto result = connection.execute("SELECT COUNT(*) FROM supplier");

      auto field = *result.getvalue(0, 0);
      auto [ptr, ec] = from_chars(field.data(), field.data() + field.size(), supplierCount);
      if (ptr != (field.data() + field.size()))
         throw Exception("unable to parse supplier count '" + string(field) + "'");
   }
   {
      // Retrieve the number of lineorders
      auto result = connection.execute("SELECT COUNT(*) FROM lineorder");

      uint64_t orderCount;
      auto field = *result.getvalue(0, 0);
      auto [ptr, ec] = from_chars(field.data(), field.data() + field.size(), orderCount);
      if (ptr != (field.data() + field.size()))
         throw Exception("unable to parse order count '" + string(field) + "'");
      nextOrderKey = orderCount + 1;
   }

   newOrder = connection.prepare("neworder", "CALL neworder($1::INTEGER, $2::VARCHAR(25), "
                                             "$3::INTEGER, $4::INTEGER, $5::INTEGER, $6::INTEGER, "
                                             "$7::VARCHAR(25), $8::VARCHAR(25), $9::VARCHAR(25), $10::VARCHAR(25), "
                                             "$11::VARCHAR(18), $12::VARCHAR(18), $13::VARCHAR(18), $14::VARCHAR(18), "
                                             "$15::CHAR(15), $16::CHAR(1), $17::INTEGER, $18::NUMERIC(18,2), $19::INTEGER, "
                                             "$20::NUMERIC(18,2), $21::NUMERIC(18,2), $22::INTEGER, $23::CHAR(10))");
   payment = connection.prepare("payment", "CALL payment($1::INTEGER, $2::INTEGER, $3::NUMERIC(18, 2), $4::INTEGER)");
   countOrders = connection.prepare("countorders", "CALL countorders($1::VARCHAR(25))");
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
