#pragma once
//---------------------------------------------------------------------------
#include "txbench/Random.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::hattrick {
//---------------------------------------------------------------------------
/// Transaction type generator
class TransactionGenerator {
   public:
   /// The transaction types
   enum class TransactionType {
      NewOrderAndPayment = 0,
      CountOrders = 1
   };

   private:
   /// The underlying data generator
   Random random;

   public:
   /// Constructor
   explicit TransactionGenerator(uint64_t seed);

   /// Pick a transaction type
   TransactionType pickTransactionType();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
