#include "tidb/hattrick/HattrickRefreshClient.hpp"
#include "txbench/Database.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/Statement.hpp"
#include <optional>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::tidb {
//---------------------------------------------------------------------------
/// The next order key
static std::atomic<uint64_t> nextOrderKey;
//---------------------------------------------------------------------------
HattrickRefreshClient::HattrickRefreshClient(const Database& database, const odbc::Environment& environment, unsigned clientId)
   : TiDBClient(database, environment), transactionGenerator(42 + clientId), generator(42 + clientId)
// Constructor
{
}
//---------------------------------------------------------------------------
void HattrickRefreshClient::performWork()
// Perform work
{
   unsigned pipelineDepth = database.getPipelineDepth();

   while (running()) {
      submittedTransactions += pipelineDepth;

      optional<uint64_t> lastOrder;

      for (unsigned c = 0; c < pipelineDepth; c++) {
         switch (transactionGenerator.pickTransactionType()) {
            case hattrick::TransactionGenerator::TransactionType::NewOrderAndPayment: {
               if (lastOrder.has_value()) {
                  orderKey = lastOrder.value();
                  custKey = generator.generateRand<int>(1, customerCount);
                  suppKey = generator.generateRand<int>(1, supplierCount);
                  amount = generator.generateRand(100, 10'495'000) / 100.00;

                  updateCustomer.exec().reset();
                  updateSupplier.exec().reset();
                  insertHistory.exec().reset();

                  lastOrder.reset();
               } else {
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

                  orderKey = nextOrderKey.fetch_add(1);
                  orderPriority = generator.generateOrderPriority();
                  shipPriority = to_string(generator.generateRand(0, 1));
                  quantity = generator.generateRand(1, 50);
                  revenue = (static_cast<double>(quantity) * (100 - discount)) / 100.00 * quantity;
                  suppCost = generator.generateRand(100, 100000) / 100.00;
                  tax = generator.generateRand(0, 8);
                  shipMode = generator.generateShipMode();

                  connection.exec("BEGIN");

                  custName = "Customer#" + generator.generateDigitString(generator.generateRand<int>(1, customerCount), 9);
                  getCustKey.exec().fetchInt(1, custKey).reset();
                  for (unsigned i = 0; i < 4; i++) {
                     lineNumber = i + 1;

                     partKey = generator.generateRand<int>(1, partCount);
                     getPartPrice.exec().fetchDouble(1, partPrice).reset();

                     suppName = "Supplier#" + generator.generateDigitString(generator.generateRand<int>(1, supplierCount), 9);
                     getSuppKey.exec().fetchInt(1, suppKey).reset();

                     dateName = generateDate();
                     getDateKey.exec().fetchInt(1, dateKey).reset();

                     partPrice *= quantity;

                     newOrder.exec().reset();
                  }
                  connection.exec("COMMIT");

                  lastOrder.emplace(orderKey);
               }
            } break;
            case hattrick::TransactionGenerator::TransactionType::CountOrders: {
               int custkey = generator.generateRand<int>(1, customerCount);
               custName = "Customer#" + generator.generateDigitString(custkey, 9);

               countOrders.exec().reset();
            } break;
         }
      }

      processedTransactions += pipelineDepth;
   }
}
//---------------------------------------------------------------------------
void HattrickRefreshClient::initialize()
// Initialize the client
{
   TiDBClient::initialize();

   auto count = [this](const string& table) {
      odbc::Statement statement;
      statement.allocate(connection);
      statement.exec("SELECT COUNT(*) FROM " + table);

      SQLINTEGER count;
      statement.bindColumn(1, count);
      if (!statement.next())
         throw Exception("no data returned from query"sv);

      return count;
   };

   customerCount = count("customer");
   partCount = count("part");
   supplierCount = count("supplier");
   nextOrderKey = count("lineorder") + 1;

   getCustKey.prepare(connection, "SELECT c_custkey FROM customer WHERE c_name = ?");
   getCustKey.bind(&custName);

   getPartPrice.prepare(connection, "SELECT p_price FROM part WHERE p_partkey = ?");
   getPartPrice.bind(&partKey);

   getSuppKey.prepare(connection, "SELECT s_suppkey FROM supplier WHERE s_name = ?");
   getSuppKey.bind(&suppName);

   getDateKey.prepare(connection, "SELECT d_datekey FROM date WHERE d_date = ?");
   getDateKey.bind(&dateName);

   newOrder.prepare(connection, "INSERT INTO lineorder(lo_orderkey, lo_linenumber, lo_custkey, lo_partkey, lo_suppkey, lo_orderdate, lo_ordpriority, lo_shippriority, "
                                "lo_quantity, lo_extendedprice, lo_discount, lo_revenue, lo_supplycost, lo_tax, lo_commitdate, lo_shipmode)\n"
                                "   VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
   newOrder.bind(&orderKey, &lineNumber, &custKey, &partKey, &suppKey, &dateKey, &orderPriority, &shipPriority, &quantity, &partPrice, &discount, &revenue, &suppCost, &tax, &dateKey, &shipMode);

   updateCustomer.prepare(connection, "update customer set c_paymentcnt = c_paymentcnt + 1 where c_custkey = ?");
   updateCustomer.bind(&custKey);

   updateSupplier.prepare(connection, "update supplier set s_ytd = s_ytd + ? where s_suppkey = ?");
   updateSupplier.bind(&amount, &suppKey);

   insertHistory.prepare(connection, "insert into history(h_orderkey, h_custkey, h_amount) values (?, ?, ?)");
   insertHistory.bind(&orderKey, &custKey, &amount);

   countOrders.prepare(connection, "select count(*) from lineorder where lo_custkey = (select c_custkey from customer where c_name = ?)");
   countOrders.bind(&custName);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
