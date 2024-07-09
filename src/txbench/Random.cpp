#include "txbench/Random.hpp"
#include <iomanip>
#include <sstream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
string Random::generateDigitString(uint64_t number, uint64_t digits)
// Generate a digit string
{
   ostringstream out;
   out << setw(digits) << setfill('0') << number;
   return out.str();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
