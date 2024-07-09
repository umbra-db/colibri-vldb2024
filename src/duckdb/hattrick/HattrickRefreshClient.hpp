#pragma once
//---------------------------------------------------------------------------
#include "duckdb/DuckDBClient.hpp"
#include "txbench/hattrick/Generator.hpp"
#include "txbench/hattrick/TransactionGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
/// HATTRICK client
class HattrickRefreshClient : public DuckDBClient {
   /// The transaction generator
   hattrick::TransactionGenerator transactionGenerator;
   /// The data generator
   hattrick::Generator generator;

   /// The get customer key statement
   DuckDBStatement getCustKey;
   /// The get part price statement
   DuckDBStatement getPartPrice;
   /// The get supplier key statement
   DuckDBStatement getSuppKey;
   /// The get customer key statement
   DuckDBStatement getDateKey;
   /// The new order statement
   DuckDBStatement newOrder;

   /// The update customer statement
   DuckDBStatement updateCustomer;
   /// The update supplier statement
   DuckDBStatement updateSupplier;
   /// The insert history statement
   DuckDBStatement insertHistory;

   /// The count orders statement
   DuckDBStatement countOrders;

   /// The customer key
   int custKey;
   /// The customer names
   std::string custName;

   /// The part price
   int partPrice;
   /// The part key
   int partKey;

   /// The supplier key
   int suppKey;
   /// The supplier name
   std::string suppName;

   /// The date key
   int dateKey;
   /// The date name
   std::string dateName;

   /// The order key
   int orderKey;
   /// The line number
   int lineNumber;
   /// The order priority
   std::string orderPriority;
   /// The ship priority
   std::string shipPriority;
   /// The quantity
   int quantity;
   /// The discount
   int discount;
   /// The revenue
   int revenue;
   /// The supply cost
   int suppCost;
   /// The tax
   int tax;
   /// The ship mode
   std::string shipMode;

   /// The amount
   int amount;

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
