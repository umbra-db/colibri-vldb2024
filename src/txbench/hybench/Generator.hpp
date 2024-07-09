#pragma once
//---------------------------------------------------------------------------
#include "txbench/Random.hpp"
#include "txbench/hybench/Config.hpp"

#include <chrono>
#include <string_view>
#include <unordered_map>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::hybench {
//---------------------------------------------------------------------------
/// Bundles generic generation functionality for HATTRICK, used both by the data generator and the driver programs
class Generator : public Random {
   /// A date
   using date = std::chrono::time_point<std::chrono::system_clock>;

   /// The config
   Config& config;

   /// The provinces
   std::vector<std::string> provinces;
   /// The cities
   std::unordered_map<std::string, std::vector<std::string>> cities;

   public:
   /// Constructor
   explicit Generator(Config& config, uint64_t seed);

   /// Generate a random gender
   std::string generateGender();
   /// Generate a random name
   std::string generateName(const std::string& gender);
   /// Generate a random customer id
   uint64_t generateCustomerId();
   /// Generate a random company id
   uint64_t generateCompanyId();
   /// Generate a random customer or company id
   uint64_t generateCustomerOrCompanyId();
   /// Generate a random age
   int generateAge();
   /// Generate a random phone number
   std::string generatePhone();
   /// Generate a random province
   std::string generateProvince();
   /// Generate a random city
   std::string generateCity(const std::string& province);

   /// Generate a random date
   date generateDateBeforeMidPoint();
   /// Generate a random date
   date generateDateAfterMidPoint();

   /// Generate a random company category
   std::string generateCompanyCategory();
   /// Generate a random company staff
   int generateCompanyStaff();

   /// Generate a random customer transfer type
   std::string generateCustomerTransferType();
   /// Generate a random company transfer type
   std::string generateCompanyTransferType();
   /// Generate a random customer checking type
   std::string generateCustomerCheckingType();
   /// Generate a random company checking type
   std::string generateCompanyCheckingType();

   /// Generate a random loan date
   date generateLoanDate();
   /// Generate a random loan duration
   int generateLoanDuration();
   /// Generate a random loan status
   std::string generateLoanStatus();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
