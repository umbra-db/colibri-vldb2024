#pragma once
//---------------------------------------------------------------------------
#include "txbench/Random.hpp"
#include "txbench/hybench/Config.hpp"
#include "txbench/hybench/Generator.hpp"
#include "txbench/io/CsvWriter.hpp"
#include <string>
#include <unordered_set>
#include <vector>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
class HyBenchGenerator {
   /// A reservoir sample
   struct ReservoirSample {
      /// The size of the sample
      unsigned size;
      /// The number of samples
      uint64_t count;
      /// The sample
      std::vector<int> samples;
      /// The random generator
      Random& random;

      /// Constructor
      ReservoirSample(unsigned size, Random& random) : size(size), count(0), random(random) {
         samples.resize(size);
      }

      /// Add an entry
      void add(int l) {
         auto pos = (count < size) ? count++ : random.generateRand(0ul, count++);
         if (pos < samples.size()) samples[pos] = l;
      }

      /// Get the sample
      const std::vector<int>& get() const { return samples; }
   };

   /// The scale factor
   unsigned scaleFactor;
   /// The output directory
   const std::string& basedir;

   CsvWriter customerWriter;
   CsvWriter savingAccountWriter;
   CsvWriter checkingAccountWriter;
   CsvWriter companyWriter;
   CsvWriter transferWriter;
   CsvWriter checkingWriter;
   CsvWriter loanTransWriter;
   CsvWriter loanAppsWriter;

   /// The blocked checking writer
   CsvWriter blockedCheckingWriter;
   /// The blocked transfer writer
   CsvWriter blockedTransferWriter;

   /// The sample of blocked transfers
   ReservoirSample blockedTransferSample;
   /// The sample of blocked checkings
   ReservoirSample blockedCheckingSample;

   /// The config
   hybench::Config config;
   /// The generator
   hybench::Generator generator;

   /// The block ids
   std::unordered_set<int> blockedIds;

   /// Generate the customer table
   void generateCustomerWithAccounts();
   /// Generate the company table
   void generateCompanyWithAccounts();
   /// Generate the transfer table
   void generateTransfer();
   /// Generate the checking table
   void generateChecking();
   /// Generate the loan table
   void generateLoan();

   public:
   /// Constructor
   HyBenchGenerator(unsigned scaleFactor, const std::string& basedir);

   /// Generate data
   virtual void generate();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
