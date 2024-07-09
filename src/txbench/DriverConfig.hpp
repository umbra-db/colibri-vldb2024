#pragma once
//---------------------------------------------------------------------------
#include "txbench/Status.hpp"
#include "txbench/io/JsonParser.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
/// Driver configuration
class DriverConfig {
   protected:
   /// The warmup time
   unsigned warmupTime = 0;
   /// The measurement time
   unsigned measureTime = 0;

   /// Read the values
   virtual Status readValues(JsonParser& parser);

   public:
   /// Destructor
   virtual ~DriverConfig() = default;

   /// Read a configuration file
   Status read(const std::string& filename);

   /// Get the warmup time
   unsigned getWarmupTime() const { return warmupTime; }
   /// Get the measurement time
   unsigned getMeasureTime() const { return measureTime; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
