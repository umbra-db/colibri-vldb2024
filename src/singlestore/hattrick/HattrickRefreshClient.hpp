#pragma once
//---------------------------------------------------------------------------
#include "singlestore/SingleStoreClient.hpp"
#include "txbench/hattrick/Generator.hpp"
#include "txbench/hattrick/TransactionGenerator.hpp"
#include "txbench/odbc/PreparedStatement.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// HATTRICK client
class HattrickRefreshClient : public SingleStoreClient {
   /// The transaction generator
   hattrick::TransactionGenerator transactionGenerator;
   /// The data generator
   hattrick::Generator generator;

   /// The new order statement
   odbc::PreparedStatement dispatch;

   /// The transaction numbers
   std::vector<SQLINTEGER> transactionNumbers;
   /// The order keys
   std::vector<SQLINTEGER> orderKeys;
   /// The customer names
   std::vector<SQLSTRING<25>> custNames;
   /// The part keys
   std::vector<SQLINTEGER> partKeys1;
   std::vector<SQLINTEGER> partKeys2;
   std::vector<SQLINTEGER> partKeys3;
   std::vector<SQLINTEGER> partKeys4;
   /// The supplier names
   std::vector<SQLSTRING<25>> suppNames1;
   std::vector<SQLSTRING<25>> suppNames2;
   std::vector<SQLSTRING<25>> suppNames3;
   std::vector<SQLSTRING<25>> suppNames4;
   /// The dates
   std::vector<SQLSTRING<18>> dates1;
   std::vector<SQLSTRING<18>> dates2;
   std::vector<SQLSTRING<18>> dates3;
   std::vector<SQLSTRING<18>> dates4;
   /// The order priorities
   std::vector<SQLSTRING<15>> ordPriorities;
   /// The ship priorities
   std::vector<SQLSTRING<1>> shipPriorities;
   /// The quantities
   std::vector<SQLINTEGER> quantities;
   /// The extended prices
   std::vector<SQLDOUBLE> extendedPrices;
   /// The discounts
   std::vector<SQLINTEGER> discounts;
   /// The revenues
   std::vector<SQLDOUBLE> revenues;
   /// The supply costs
   std::vector<SQLDOUBLE> supplyCosts;
   /// The taxes
   std::vector<SQLINTEGER> taxes;
   /// The ship modes
   std::vector<SQLSTRING<10>> shipModes;

   /// The customer keys
   std::vector<SQLINTEGER>& custKeys = partKeys1;
   /// The supplier keys
   std::vector<SQLINTEGER>& suppKeys = partKeys2;
   /// The amounts
   std::vector<SQLDOUBLE>& amounts = revenues;

   /// The number of customers
   uint64_t customerCount;
   /// The number of parts
   uint64_t partCount;
   /// The number of suppliers
   uint64_t supplierCount;

   /// Clear the dispatch data vectors
   void clearDispatchData();

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
