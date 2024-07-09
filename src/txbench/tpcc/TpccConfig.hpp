#pragma once
//---------------------------------------------------------------------------
#include "txbench/BenchmarkConfig.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tpcc {
//---------------------------------------------------------------------------
/// TPCC configuration
class TpccConfig : public BenchmarkConfig {
   /// Whether to generate remote accesses
   bool generateRemoteAccesses;
   /// Whether to generate user rollbacks
   bool generateUserRollbacks;

   protected:
   /// Read the values
   Status readValues(JsonParser& parser) override;

   public:
   /// Whether to generate remote accesses
   bool shouldGenerateRemoteAccesses() const { return generateRemoteAccesses; }
   /// Whether to generate user rollbacks
   bool shouldGenerateUserRollbacks() const { return generateUserRollbacks; }
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
