#include "txbench/hybench/QueryGenerator.hpp"
#include <algorithm>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::hybench {
//---------------------------------------------------------------------------
QueryGenerator::QueryGenerator(uint64_t seed, unsigned size)
   : random(seed)
// Constructor
{
   for (unsigned i = 0; i < size; i++)
      queries.push_back(i);
}
//---------------------------------------------------------------------------
unsigned QueryGenerator::pickQuery()
// Pick a transaction type
{
   if (position == queries.size()) {
      position = 0;
   }

   if (position == 0) {
      std::shuffle(queries.begin(), queries.end(), random.getEngine());
   }

   return queries[position++];
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
