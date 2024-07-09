#include "txbench/tpcc/TpccTransactionGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tpcc {
//---------------------------------------------------------------------------
TpccTransactionGenerator::TpccTransactionGenerator(uint64_t seed)
   : random(seed)
// Constructor
{
}
//---------------------------------------------------------------------------
TpccTransactionGenerator::TransactionType TpccTransactionGenerator::pickTransactionType()
// Pick a transaction type
{
   auto dice = random.generateRand(1, 100);
   if (dice <= 42)
      return TransactionType::Payment;

   dice -= 42;
   if (dice <= 4)
      return TransactionType::OrderStatus;

   dice -= 4;
   if (dice <= 4)
      return TransactionType::Delivery;

   dice -= 4;
   if (dice <= 4)
      return TransactionType::StockLevel;

   dice -= 4;
   return TransactionType::NewOrder;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
