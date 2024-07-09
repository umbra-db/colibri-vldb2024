#pragma once
//---------------------------------------------------------------------------
#include "txbench/Random.hpp"
#include <array>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tpch {
//---------------------------------------------------------------------------
/// Transaction type generator
class TpchQueryGenerator {
   /// The underlying data generator
   Random random;
   /// The queries
   std::array<unsigned, 22> queries;
   /// The current position
   unsigned position = 0;

   public:
   /// Constructor
   explicit TpchQueryGenerator(uint64_t seed);

   /// Pick a transaction type
   unsigned pickQuery();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
