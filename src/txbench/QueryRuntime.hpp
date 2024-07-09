#pragma once
//---------------------------------------------------------------------------
#include <chrono>
#include <thread>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
class DatabaseClient;
//---------------------------------------------------------------------------
/// A query runtime
struct QueryRuntime {
   /// The combined runtime
   double runtime = 0;
   /// The count of recorded queries
   uint64_t count = 0;
   /// The minimum runtime
   double minimum = 0;
   /// The maximum runtime
   double maximum = 0;

   /// Add a new execution time
   void add(double executionTime) {
      if (count == 0) minimum = executionTime;

      runtime += executionTime;
      count++;
      minimum = std::min(minimum, executionTime);
      maximum = std::max(maximum, executionTime);
   }
   /// Merge with another query runtime
   void merge(const QueryRuntime& other) {
      if (other.count == 0) return;
      if (count == 0) minimum = other.minimum;

      runtime += other.runtime;
      count += other.count;
      minimum = std::min(minimum, other.minimum);
      maximum = std::max(maximum, other.maximum);
   }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
