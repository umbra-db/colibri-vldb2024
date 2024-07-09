#include "txbench/hattrick/TransactionGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::hattrick {
//---------------------------------------------------------------------------
TransactionGenerator::TransactionGenerator(uint64_t seed)
   : random(seed)
// Constructor
{
}
//---------------------------------------------------------------------------
TransactionGenerator::TransactionType TransactionGenerator::pickTransactionType()
// Pick a transaction type
{
   auto dice = random.generateRand(1, 100);
   if (dice <= 96)
      return TransactionType::NewOrderAndPayment;
   else
      return TransactionType::CountOrders;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
