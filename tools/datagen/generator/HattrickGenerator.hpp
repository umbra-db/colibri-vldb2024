#pragma once
//---------------------------------------------------------------------------
#include "txbench/hattrick/Generator.hpp"
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
class HattrickGenerator {
   protected:
   /// The scale factor
   uint64_t scaleFactor;
   /// The output directory
   const std::string& basedir;

   /// The number of customers
   uint64_t customerCount;
   /// The number of suppliers
   uint64_t supplierCount;
   /// The number of parts
   uint64_t partCount;
   /// The number of lineorders
   uint64_t lineorderCount;

   /// The payment counts per customer
   std::vector<unsigned> paymentCounts;
   /// The ytd per supplier
   std::vector<double> ytds;

   private:
   /// The generator
   hattrick::Generator generator;

   /// Generate the part table
   void generatePart();
   /// Generate the date table
   void generateDate();
   /// Generate the lineorder table
   void generateLineorder();
   /// Generate the customer table
   void generateCustomer();
   /// Generate the supplier tables
   void generateSupplier();

   public:
   /// Constructor
   HattrickGenerator(unsigned scaleFactor, const std::string& basedir);

   /// Generate data
   virtual void generate();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
