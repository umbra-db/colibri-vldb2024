#include "txbench/tatp/TatpGenerator.hpp"
#include <algorithm>
#include <iomanip>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::tatp {
//---------------------------------------------------------------------------
TatpGenerator::SampleGenerator::SampleGenerator(TatpGenerator& generator, vector<uint64_t> values)
   : generator(&generator), values(std::move(values))
// Constructor
{
}
//---------------------------------------------------------------------------
span<uint64_t> TatpGenerator::SampleGenerator::sample(uint64_t count)
// Generate
{
   shuffle(values.begin(), values.end(), generator->rng);
   return {values.data(), count};
}
//---------------------------------------------------------------------------
TatpGenerator::TatpGenerator(uint64_t seed)
   : Random(seed)
// Constructor
{
}
//---------------------------------------------------------------------------
TatpGenerator::SampleGenerator TatpGenerator::getSampleGenerator(vector<uint64_t> values)
// Get a sample generator
{
   return SampleGenerator{*this, std::move(values)};
}
//---------------------------------------------------------------------------
string TatpGenerator::generateAlphaString(uint64_t length)
// Generate a random alphabetic string
{
   uniform_int_distribution charDist(0, static_cast<int>(alphabetic.size() - 1));

   string result(length, ' ');
   for (auto& c : result)
      c = alphabetic[charDist(rng)];
   return result;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
