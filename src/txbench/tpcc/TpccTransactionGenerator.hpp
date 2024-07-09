#pragma once
//---------------------------------------------------------------------------
#include "txbench/Random.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tpcc {
//---------------------------------------------------------------------------
/// Transaction type generator
class TpccTransactionGenerator {
   public:
   /// The transaction types
   enum class TransactionType {
      Delivery = 0,
      NewOrder = 1,
      OrderStatus = 2,
      Payment = 3,
      StockLevel = 4
   };

   private:
   /// The underlying data generator
   Random random;

   public:
   /// Constructor
   explicit TpccTransactionGenerator(uint64_t seed);

   /// Pick a transaction type
   TransactionType pickTransactionType();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
