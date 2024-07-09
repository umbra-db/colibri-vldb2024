#include "txbench/tpcc/TpccGenerator.hpp"
#include <ctime>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::tpcc {
//---------------------------------------------------------------------------
TpccGenerator::TpccGenerator(uint64_t seed)
   : Random(seed)
// Constructor
{
}
//---------------------------------------------------------------------------
string TpccGenerator::generateNumString(unsigned length)
// Generate a random n-string
{
   return generateNumString(length, length);
}
//---------------------------------------------------------------------------
string TpccGenerator::generateNumString(unsigned minLength, unsigned maxLength)
// Generate a random n-string
{
   string result(generateRand(minLength, maxLength), ' ');
   for (auto& c : result)
      c = numeric[generateRand(0, 9)];
   return result;
}
//---------------------------------------------------------------------------
string TpccGenerator::generateAlphanumString(unsigned length)
// Generate a random a-string
{
   return generateAlphanumString(length, length);
}
//---------------------------------------------------------------------------
string TpccGenerator::generateAlphanumString(unsigned minLength, unsigned maxLength)
// Generate a random a-string
{
   string result(generateRand(minLength, maxLength), ' ');
   for (auto& c : result)
      c = alphanumeric[generateRand(0, 61)];
   return result;
}
//---------------------------------------------------------------------------
string TpccGenerator::generateZipCode()
// Generate a random zip code
{
   string result(9, '1');
   for (unsigned i = 0; i < 4; ++i)
      result[i] = numeric[generateRand(0, 9)];
   return result;
}
//---------------------------------------------------------------------------
string TpccGenerator::generateData()
// Generate a random data string
{
   uniform_real_distribution originalDist;

   string data = generateAlphanumString(26, 50);
   if (generateReal() < 0.1) {
      data.replace(generateRand(0ull, data.size() - 8ull), 8, "ORIGINAL");
   }

   return data;
}
//---------------------------------------------------------------------------
string TpccGenerator::generateCustomerLastname(unsigned index)
// Make a customer lastname
{
   unsigned index1 = index % 10;
   unsigned index2 = (index / 10) % 10;
   unsigned index3 = (index / 100) % 10;

   string result(lastnameSyllables[index1].size() + lastnameSyllables[index2].size() + lastnameSyllables[index3].size(), ' ');
   result.replace(0, lastnameSyllables[index1].size(), lastnameSyllables[index1]);
   result.replace(lastnameSyllables[index1].size(), lastnameSyllables[index2].size(), lastnameSyllables[index2]);
   result.replace(lastnameSyllables[index1].size() + lastnameSyllables[index2].size(), lastnameSyllables[index3].size(), lastnameSyllables[index3]);

   return result;
}
//---------------------------------------------------------------------------
string TpccGenerator::generateDatetime()
// Generate the current date and time
{
   time_t now = {};
   time(&now);
   string result(19, ' ');
   strftime(&result[0], 20, "%Y-%m-%d %H:%M:%S", gmtime(&now));
   return result;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
