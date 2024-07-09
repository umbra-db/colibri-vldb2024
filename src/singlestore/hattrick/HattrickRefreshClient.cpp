#include "singlestore/hattrick/HattrickRefreshClient.hpp"
#include "txbench/Database.hpp"
#include "txbench/Exception.hpp"
#include "txbench/odbc/Statement.hpp"
#include <optional>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// The next order key
static std::atomic<uint64_t> nextOrderKey;
//---------------------------------------------------------------------------
HattrickRefreshClient::HattrickRefreshClient(const Database& database, const odbc::Environment& environment, unsigned clientId)
   : SingleStoreClient(database, environment), transactionGenerator(42 + clientId), generator(42 + clientId), dispatch(database.getPipelineDepth())
// Constructor
{
   clearDispatchData();
}
//---------------------------------------------------------------------------
void HattrickRefreshClient::clearDispatchData()
// Clear the dispatch data
{
   unsigned pipelineDepth = database.getPipelineDepth();

   auto reset = [pipelineDepth](auto& data) {
      data.clear();
      data.resize(pipelineDepth);
   };

   reset(transactionNumbers);
   reset(orderKeys);
   reset(custNames);
   reset(partKeys1);
   reset(partKeys2);
   reset(partKeys3);
   reset(partKeys4);
   reset(suppNames1);
   reset(suppNames2);
   reset(suppNames3);
   reset(suppNames4);
   reset(dates1);
   reset(dates2);
   reset(dates3);
   reset(dates4);
   reset(ordPriorities);
   reset(shipPriorities);
   reset(quantities);
   reset(extendedPrices);
   reset(discounts);
   reset(revenues);
   reset(supplyCosts);
   reset(taxes);
   reset(shipModes);
}
//---------------------------------------------------------------------------
void HattrickRefreshClient::performWork()
// Perform work
{
   unsigned pipelineDepth = database.getPipelineDepth();

   while (running()) {
      submittedTransactions += pipelineDepth;

      optional<uint64_t> lastOrder;

      for (unsigned i = 0; i < pipelineDepth; ++i) {
         switch (transactionGenerator.pickTransactionType()) {
            case hattrick::TransactionGenerator::TransactionType::NewOrderAndPayment: {
               if (lastOrder.has_value()) {
                  int custkey = generator.generateRand<int>(1, customerCount);
                  int suppkey = generator.generateRand<int>(1, supplierCount);
                  double amount = generator.generateRand(100, 10'495'000) / 100.00;

                  transactionNumbers[i] = 1;
                  custKeys[i] = custkey;
                  suppKeys[i] = suppkey;
                  amounts[i] = amount;
                  orderKeys[i] = lastOrder.value();

                  lastOrder.reset();
               } else {
                  auto orderkey = nextOrderKey.fetch_add(1);
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
                  double extendedprice = quantity;
                  int discount = generator.generateRand(0, 10);
                  double revenue = (static_cast<double>(quantity) * (100 - discount)) / 100.00;
                  double supplycost = generator.generateRand(100, 100000) / 100.00;
                  int tax = generator.generateRand(0, 8);
                  string shipmode = generator.generateShipMode();

                  transactionNumbers[i] = 0;
                  orderKeys[i] = orderkey;
                  custNames[i] = customerName;
                  partKeys1[i] = partkey1;
                  partKeys2[i] = partkey2;
                  partKeys3[i] = partkey3;
                  partKeys4[i] = partkey4;
                  suppNames1[i] = supplierName1;
                  suppNames2[i] = supplierName2;
                  suppNames3[i] = supplierName3;
                  suppNames4[i] = supplierName4;
                  dates1[i] = date1;
                  dates2[i] = date2;
                  dates3[i] = date3;
                  dates4[i] = date4;
                  ordPriorities[i] = ordpriority;
                  shipPriorities[i] = shippriority;
                  quantities[i] = quantity;
                  extendedPrices[i] = extendedprice;
                  discounts[i] = discount;
                  revenues[i] = revenue;
                  supplyCosts[i] = supplycost;
                  taxes[i] = tax;
                  shipModes[i] = shipmode;

                  lastOrder.emplace(orderkey);
               }
            } break;
            case hattrick::TransactionGenerator::TransactionType::CountOrders: {
               int custkey = generator.generateRand<int>(1, customerCount);
               string customerName = "Customer#" + generator.generateDigitString(custkey, 9);

               transactionNumbers[i] = 2;
               custNames[i] = customerName;
            } break;
         }
      }

      dispatch.exec();

      processedTransactions += pipelineDepth;
   }
}
//---------------------------------------------------------------------------
void HattrickRefreshClient::initialize()
// Initialize the client
{
   SingleStoreClient::initialize();

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

   dispatch.prepare(connection, "{CALL dispatch(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)}");
   dispatch.bind(
      transactionNumbers.data(),
      orderKeys.data(),
      custNames.data(),
      partKeys1.data(),
      partKeys2.data(),
      partKeys3.data(),
      partKeys4.data(),
      suppNames1.data(),
      suppNames2.data(),
      suppNames3.data(),
      suppNames4.data(),
      dates1.data(),
      dates2.data(),
      dates3.data(),
      dates4.data(),
      ordPriorities.data(),
      shipPriorities.data(),
      quantities.data(),
      extendedPrices.data(),
      discounts.data(),
      revenues.data(),
      supplyCosts.data(),
      taxes.data(),
      shipModes.data());
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
