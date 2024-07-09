#pragma once
//---------------------------------------------------------------------------
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
/// Progress printing helper
class ProgressPrinter {
   private:
   /// The message
   std::string message;
   /// The maximum count
   unsigned maximum;
   /// The current percentage
   unsigned currentPercentage;

   public:
   /// Constructor
   ProgressPrinter(std::string message, unsigned maximum);
   /// Destructor
   ~ProgressPrinter();

   /// Update
   void update(unsigned current);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
