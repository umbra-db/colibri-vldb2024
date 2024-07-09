#pragma once
//---------------------------------------------------------------------------
#include "generator/TpccGenerator.hpp"
#include "txbench/tpcch/TpcchGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
class TpcchGenerator : public TpccGenerator {
   private:
   /// The generator
   tpcch::TpcchGenerator generator;

   /// Generate the region table
   void generateRegion();
   /// Generate the nation table
   void generateNation();
   /// Generate the supplier table
   void generateSupplier();

   public:
   /// Constructor
   TpcchGenerator(unsigned scaleFactor, const std::string& basedir);

   /// Generate data
   void generate() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
