#include "txbench/tpch/TpchQueryGenerator.hpp"
#include <algorithm>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tpch {
//---------------------------------------------------------------------------
TpchQueryGenerator::TpchQueryGenerator(uint64_t seed)
   : random(seed)
// Constructor
{
   for (unsigned i = 0; i < queries.size(); i++)
      queries[i] = i;
}
//---------------------------------------------------------------------------
unsigned TpchQueryGenerator::pickQuery()
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
