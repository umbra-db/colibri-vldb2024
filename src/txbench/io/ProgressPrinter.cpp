#include "txbench/io/ProgressPrinter.hpp"
#include <iomanip>
#include <iostream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
ProgressPrinter::ProgressPrinter(string message, unsigned maximum)
   : message(move(message)),
     maximum(maximum),
     currentPercentage(0)
// Constructor
{
   cout << setw(20) << message << " [" << setw(3) << currentPercentage << "%]" << flush;
}
//---------------------------------------------------------------------------
ProgressPrinter::~ProgressPrinter()
// Destructor
{
   cout << endl;
}
//---------------------------------------------------------------------------
void ProgressPrinter::update(unsigned current)
// Update
{
   auto newPercentage = 100 * static_cast<double>(current) / maximum;
   if ((current == maximum) || (newPercentage > currentPercentage)) {
      currentPercentage = newPercentage;
      cout << "\r" << left << setw(21) << message << " [" << setw(3) << currentPercentage << "%]" << flush;
   }
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------