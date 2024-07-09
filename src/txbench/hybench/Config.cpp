#include "txbench/hybench/Config.hpp"
#include <chrono>
#include <sstream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::hybench {
//---------------------------------------------------------------------------
std::string Config::getEndDate()
// Get the end date
{
   auto end = std::chrono::system_clock::from_time_t(std::mktime(&endDate));
   std::stringstream ss;
   ss << end;
   return ss.str();
}
//---------------------------------------------------------------------------
Config Config::instantiate(unsigned scaleFactor)
// Instantiate the config
{
   Config config;
   config.customerNumber = 300'000 * scaleFactor;
   config.companyNumber = 2'000 * scaleFactor;
   config.transferNumber = 6'000'000 * scaleFactor;
   config.checkingNumber = 600'000 * scaleFactor;
   config.loanappNumber = 600'000 * scaleFactor;
   config.loansNumber = 600'000 * scaleFactor;

   config.startYear = 2014;
   config.midPoint = 2019;
   config.endYear = 2024;

   config.startDate = {};
   config.midPointDate = {};
   config.endDate = {};
   config.loanDate = {};
   strptime("2014-01-01", "%Y-%m-%d", &config.startDate);
   strptime("2019-01-01", "%Y-%m-%d", &config.midPointDate);
   strptime("2023-12-31", "%Y-%m-%d", &config.endDate);
   strptime("2023-10-01", "%Y-%m-%d", &config.loanDate);

   config.customerAgeLower = 18;
   config.customerAgeUpper = 60;
   config.customerLoanBalance = 10'000;
   config.customerSavingBalance = 2'000;
   config.customerCheckingBalance = 5'000;

   config.companySizeLower = 10;
   config.companySizeUpper = 100;
   config.companyLoanBalance = 1'000'000;
   config.companySavingBalance = 10'000;
   config.companyCheckingBalance = 100'000;

   config.custRate = 0.8;
   config.loanRate = 0.2;
   config.probBlocked = 0.01;
   config.riskRate = 0.1;

   return config;
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
