#pragma once
//---------------------------------------------------------------------------
#include "txbench/Random.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::hybench {
//---------------------------------------------------------------------------
/// Transaction type generator
class TransactionGenerator {
   /// The underlying data generator
   Random random;
   /// Whether to generate OLTP or OLXP transactions
   bool oltp;

   public:
   /// Constructor
   explicit TransactionGenerator(uint64_t seed, bool oltp);

   /// Pick a transaction
   unsigned pickTransaction();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
