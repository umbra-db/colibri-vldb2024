#pragma once
//---------------------------------------------------------------------------
#include "txbench/tatp/TatpGenerator.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
/// TATP data generator
class TatpGenerator {
   private:
   /// The scale factor
   unsigned scaleFactor;
   /// The output directory
   const std::string& basedir;

   /// The generator
   tatp::TatpGenerator generator;

   /// Generate subscriber data
   void generateSubscriber();
   /// Generate access info data
   void generateAccessInfo();
   /// Generate special facility and call forwarding data
   void generateSpecialFacility();

   public:
   /// Constructor
   TatpGenerator(unsigned scaleFactor, const std::string& basedir);

   /// Generate data
   void generate();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
