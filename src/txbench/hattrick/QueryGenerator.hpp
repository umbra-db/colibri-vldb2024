#pragma once
//---------------------------------------------------------------------------
#include "txbench/Random.hpp"
#include <array>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::hattrick {
//---------------------------------------------------------------------------
/// Transaction type generator
class QueryGenerator {
   private:
   /// The underlying data generator
   Random random;
   /// The queries
   std::array<unsigned, 13> queries;
   /// The current position
   unsigned position = 0;

   public:
   /// Constructor
   explicit QueryGenerator(uint64_t seed);

   /// Pick a transaction type
   unsigned pickQuery();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
