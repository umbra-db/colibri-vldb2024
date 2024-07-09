#pragma once
//---------------------------------------------------------------------------
#include "txbench/Random.hpp"
#include <array>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::hybench {
//---------------------------------------------------------------------------
/// Transaction type generator
class QueryGenerator {
   private:
   /// The underlying data generator
   Random random;
   /// The queries
   std::vector<unsigned> queries;
   /// The current position
   unsigned position = 0;

   public:
   /// Constructor
   explicit QueryGenerator(uint64_t seed, unsigned size);

   /// Pick a transaction type
   unsigned pickQuery();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
