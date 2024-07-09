#include "txbench/hybench/TransactionGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::hybench {
//---------------------------------------------------------------------------
TransactionGenerator::TransactionGenerator(uint64_t seed, bool oltp)
   : random(seed), oltp(oltp)
// Constructor
{
}
//---------------------------------------------------------------------------
unsigned TransactionGenerator::pickTransaction()
// Pick a transaction
{
   auto dice = random.generateRand(1, 100);
   if (oltp) {
      if (dice <= 5)
         return 0;
      else if (dice <= 8)
         return 1;
      else if (dice <= 13)
         return 2;
      else if (dice <= 18)
         return 3;
      else if (dice <= 21)
         return 4;
      else if (dice <= 24)
         return 5;
      else if (dice <= 27)
         return 6;
      else if (dice <= 30)
         return 7;
      else if (dice <= 37)
         return 8;
      else if (dice <= 44)
         return 9;
      else if (dice <= 51)
         return 10;
      else if (dice <= 58)
         return 11;
      else if (dice <= 64)
         return 12;
      else if (dice <= 68)
         return 13;
      else if (dice <= 72)
         return 14;
      else if (dice <= 76)
         return 15;
      else if (dice <= 88)
         return 16;
      else
         return 17;
   } else {
      if (dice <= 35)
         return 0;
      else if (dice <= 60)
         return 1;
      else if (dice <= 75)
         return 2;
      else if (dice <= 90)
         return 3;
      else if (dice <= 97)
         return 4;
      else
         return 5;
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
