#pragma once
//---------------------------------------------------------------------------
#include "txbench/Random.hpp"
#include <cstdint>
#include <span>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tatp {
//---------------------------------------------------------------------------
/// Bundles generic generation functionality for TATP, used both by the data generator and the driver programs
class TatpGenerator : public Random {
   public:
   /// Sample generator
   class SampleGenerator {
      /// The generator
      TatpGenerator* generator;
      /// The possible values
      std::vector<uint64_t> values;

      /// Constructor
      SampleGenerator(TatpGenerator& generator, std::vector<uint64_t> values);

      friend TatpGenerator;

      public:
      /// Sample values
      std::span<uint64_t> sample(uint64_t count);
   };

   private:
   /// Alphabetic
   static constexpr std::string_view alphabetic = std::string_view("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 26);

   public:
   /// Constructor
   explicit TatpGenerator(uint64_t seed);

   /// Get a sample generator
   SampleGenerator getSampleGenerator(std::vector<uint64_t> values);

   /// Generate a random alphabetical string
   std::string generateAlphaString(uint64_t length);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
