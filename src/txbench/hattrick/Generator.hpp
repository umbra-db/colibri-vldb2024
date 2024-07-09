#pragma once
//---------------------------------------------------------------------------
#include "txbench/Random.hpp"
#include <string_view>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::hattrick {
//---------------------------------------------------------------------------
/// Bundles generic generation functionality for HATTRICK, used both by the data generator and the driver programs
class Generator : public Random {
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

   /// The prices
   std::vector<double> prices;
   /// The dates
   std::vector<std::string> dates;

   public:
   /// Constructor
   explicit Generator(uint64_t seed);

   /// Generate a part name
   std::string generatePartName();
   /// Generate a part type
   std::string generatePartType();
   /// Generate a part container
   std::string generatePartContainer();
   /// Generate a part price
   double generatePartPrice(unsigned part);
   /// Get a part price
   double getPrice(unsigned part) { return prices[part - 1]; }

   /// Add a date string
   void addDate(std::string date) { dates.push_back(date); }
   /// Get a date
   std::string getDate(unsigned id) { return dates[id]; }

   /// Generate an order priority
   std::string generateOrderPriority();
   /// Generate a ship mode
   std::string generateShipMode();

   /// Generate an address
   std::string generateAddress();
   /// Generate a city
   std::string generateCity(unsigned nationId);
   /// Generate a nation
   std::string generateNation(unsigned nationId);
   /// Generate a region
   std::string generateRegion(unsigned nationId);
   /// Generate a phone
   std::string generatePhone(unsigned nationId);
   /// Generate a customer market segment
   std::string generateCustomerMktSegment();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
