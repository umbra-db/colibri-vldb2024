#pragma once
//---------------------------------------------------------------------------
#include "txbench/Random.hpp"
#include <random>
#include <string_view>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tpcch {
//---------------------------------------------------------------------------
/// Bundles generic generation functionality for Tpcch, used both by the data generator and the driver programs
class TpcchGenerator : public Random {
   public:
   /// Region count
   static constexpr unsigned regionCount = 5;
   /// Nation count
   static constexpr unsigned nationCount = 62;
   /// Supplier count
   static constexpr unsigned supplierCount = 10'000;

   /// A nation
   struct Nation {
      /// The nation id
      int id;
      /// The nation name
      std::string name;
      /// The region id
      int regionId;
   };

   private:
   /// Alphanumeric characters
   static constexpr char alphanumeric64[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?@";

   /// Generate a random sentence
   std::string generateSentence();
   /// Generate a random noun phrase
   std::string generateNounPhrase();
   /// Generate a random verb phrase
   std::string generateVerbPhrase();

   /// Generate a random noun
   std::string generateNoun();
   /// Generate a random verb
   std::string generateVerb();
   /// Generate a random adjective
   std::string generateAdjective();
   /// Generate a random adverb
   std::string generateAdverb();
   /// Generate a random preposition
   std::string generatePreposition();
   /// Generate a random terminator
   std::string generateTerminator();
   /// Generate a random auxiliary
   std::string generateAuxiliary();

   public:
   /// Constructor
   explicit TpcchGenerator(uint64_t seed);

   /// Generate a region name
   std::string generateRegionName(unsigned id);
   /// Generate a nation
   Nation generateNation(unsigned id);
   /// Generate a nation key
   int generateNationKey();
   /// Generate a supplier phone number
   std::string generateSupplierPhone(unsigned id);
   /// Generate a supplier comment
   std::string generateSupplierComment(int minLength, int maxLength, std::string action);

   /// Generate a random double
   std::string generateDouble(double minValue, double maxValue, unsigned decimals);
   /// Generate a random text
   std::string generateText(unsigned length);
   /// Generate a random text
   std::string generateText(unsigned minLength, unsigned maxLength);
   /// Generate a alpha-numeric text
   std::string generateAlphanumeric64(unsigned length);
   /// Generate a alpha-numeric text
   std::string generateAlphanumeric64(unsigned minLength, unsigned maxLength);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
