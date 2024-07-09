#pragma once
//---------------------------------------------------------------------------
#include "txbench/Random.hpp"
#include <string_view>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::tpcc {
//---------------------------------------------------------------------------
/// Bundles generic generation functionality for TPCC, used both by the data generator and the driver programs
class TpccGenerator : public Random {
   public:
   /// Item count
   static constexpr unsigned itemCount = 100000;
   /// Districts per warehouse
   static constexpr unsigned districtsPerWarehouse = 10;
   /// Customers per district
   static constexpr unsigned customersPerDistrict = 3000;

   private:
   /// Numeric characters
   static constexpr char numeric[] = "0123456789";
   /// Alphanumeric characters
   static constexpr char alphanumeric[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
   /// Customer lastname syllables
   static constexpr std::string_view lastnameSyllables[] = {"BAR", "OUGHT", "ABLE", "PRI", "PRES", "ESE", "ANTI", "CALLY", "ATION", "EING"};

   public:
   /// Constructor
   explicit TpccGenerator(uint64_t seed);

   /// Generate a random n-string
   std::string generateNumString(unsigned length);
   /// Generate a random n-string
   std::string generateNumString(unsigned minLength, unsigned maxLength);
   /// Generate a random a-string
   std::string generateAlphanumString(unsigned length);
   /// Generate a random a-string
   std::string generateAlphanumString(unsigned minLength, unsigned maxLength);
   /// Generate a random zipcode
   std::string generateZipCode();
   /// Generate a random data string
   std::string generateData();
   /// Generate a customer last name
   std::string generateCustomerLastname(unsigned index);
   /// Generate the current date and time
   std::string generateDatetime();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
