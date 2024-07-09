#pragma once
//---------------------------------------------------------------------------
#include <random>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench {
//---------------------------------------------------------------------------
/// Bundle functionality to random operations
class Random {
   public:
   /// The random number generator
   std::mt19937 rng;

   /// Constructor
   explicit Random(uint64_t seed) : rng(seed) {}

   /// Get the underlying engine
   std::mt19937& getEngine() { return rng; }

   /// Generate a uniform real number
   double generateReal() {
      std::uniform_real_distribution dist;
      return dist(rng);
   }
   /// Generate a uniform real number
   double generateReal(double min, double max) {
      std::uniform_real_distribution dist(min, max);
      return dist(rng);
   }
   /// Generate a uniform random number
   template <typename T>
   T generateRand(T min, T max) {
      std::uniform_int_distribution dist(min, max);
      return dist(rng);
   }
   /// Generate a non-uniform random number
   template <typename T>
   T generateNURand(T A, T min, T max) {
      std::uniform_int_distribution dist1(0u, A);
      std::uniform_int_distribution dist2(min, max);

      return (((dist1(rng) | dist2(rng)) + 42) % (max - min + 1)) + min;
   }
   /// Generate a random string
   const std::string& generateString(const std::vector<std::string>& strings) {
      return strings[generateRand(0ul, strings.size() - 1)];
   }

   /// Generate a digit string from a number
   std::string generateDigitString(uint64_t number, uint64_t digits);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
