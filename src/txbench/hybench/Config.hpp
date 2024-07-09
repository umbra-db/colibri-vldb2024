#pragma once
//---------------------------------------------------------------------------
#include <cstdint>
#include <ctime>
#include <string>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::hybench {
//---------------------------------------------------------------------------
/// HyBench config
struct Config {
   using date = std::tm;

   /// The number of customers
   uint64_t customerNumber;
   /// The number of companies
   uint64_t companyNumber;
   /// The number of transfers
   uint64_t transferNumber;
   /// The number of checkings
   uint64_t checkingNumber;
   //// The number of loan applications
   uint64_t loanappNumber;
   /// The number of loan tranfers
   uint64_t loansNumber;

   /// The start year
   int startYear;
   /// The mid point year
   int midPoint;
   /// The end year
   int endYear;

   /// The start date
   date startDate;
   /// The mid point date
   date midPointDate;
   /// The end date
   date endDate;
   /// The loan date
   date loanDate;

   /// The customer age lower
   int customerAgeLower;
   /// The customer age upper
   int customerAgeUpper;
   /// The customer loan balance
   double customerLoanBalance;
   /// The customer saving balance
   double customerSavingBalance;
   /// The customer checking balance
   double customerCheckingBalance;

   /// The company size lower
   int companySizeLower;
   /// The company size upper
   int companySizeUpper;
   /// The company loan balance
   double companyLoanBalance;
   /// The company saving balance
   double companySavingBalance;
   /// The company checking balance
   double companyCheckingBalance;

   /// The cust rate
   double custRate;
   /// The loan rate
   double loanRate;
   /// The probability that an account is blocked
   double probBlocked;
   /// The risk rate
   double riskRate;

   /// Get the end date
   std::string getEndDate();

   /// Instantiate the config
   static Config instantiate(unsigned scaleFactor);
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
