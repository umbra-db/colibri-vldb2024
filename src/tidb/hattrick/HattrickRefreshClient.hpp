#pragma once
//---------------------------------------------------------------------------
#include "tidb/TiDBClient.hpp"
#include "txbench/hattrick/Generator.hpp"
#include "txbench/hattrick/TransactionGenerator.hpp"
#include "txbench/odbc/PreparedStatement.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tidb {
//---------------------------------------------------------------------------
/// HATTRICK client
class HattrickRefreshClient : public TiDBClient {
   /// The transaction generator
   hattrick::TransactionGenerator transactionGenerator;
   /// The data generator
   hattrick::Generator generator;

   /// The get customer key statement
   odbc::PreparedStatement getCustKey;
   /// The get part price statement
   odbc::PreparedStatement getPartPrice;
   /// The get supplier key statement
   odbc::PreparedStatement getSuppKey;
   /// The get customer key statement
   odbc::PreparedStatement getDateKey;
   /// The new order statement
   odbc::PreparedStatement newOrder;

   /// The update customer statement
   odbc::PreparedStatement updateCustomer;
   /// The update supplier statement
   odbc::PreparedStatement updateSupplier;
   /// The insert history statement
   odbc::PreparedStatement insertHistory;

   /// The count orders statement
   odbc::PreparedStatement countOrders;

   /// The customer key
   SQLINTEGER custKey;
   /// The customer names
   SQLSTRING<25> custName;

   /// The part price
   SQLDOUBLE partPrice;
   /// The part key
   SQLINTEGER partKey;

   /// The supplier key
   SQLINTEGER suppKey;
   /// The supplier name
   SQLSTRING<25> suppName;

   /// The date key
   SQLINTEGER dateKey;
   /// The date name
   SQLSTRING<18> dateName;

   /// The order key
   SQLINTEGER orderKey;
   /// The line number
   SQLINTEGER lineNumber;
   /// The order priority
   SQLSTRING<15> orderPriority;
   /// The ship priority
   SQLSTRING<15> shipPriority;
   /// The quantity
   SQLINTEGER quantity;
   /// The discount
   SQLINTEGER discount;
   /// The revenue
   SQLDOUBLE revenue;
   /// The supply cost
   SQLDOUBLE suppCost;
   /// The tax
   SQLINTEGER tax;
   /// The ship mode
   SQLSTRING<10> shipMode;

   /// The amount
   SQLDOUBLE amount;

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
   explicit HattrickRefreshClient(const Database& database, const odbc::Environment& environment, unsigned clientId);

   /// Initialize the client
   void initialize() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
