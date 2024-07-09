#include "generator/HyBenchGenerator.hpp"
#include "txbench/io/ProgressPrinter.hpp"
#include <algorithm>
#include <iomanip>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::datagen {
//---------------------------------------------------------------------------
HyBenchGenerator::HyBenchGenerator(unsigned scaleFactor, const string& basedir)
   : scaleFactor(scaleFactor), basedir(std::move(basedir)), customerWriter(basedir + "/customer.csv", ','), savingAccountWriter(basedir + "/savingaccount.csv", ','),
     checkingAccountWriter(basedir + "/checkingaccount.csv", ','), companyWriter(basedir + "/company.csv", ','), transferWriter(basedir + "/transfer.csv", ','),
     checkingWriter(basedir + "/checking.csv", ','), loanTransWriter(basedir + "/loantrans.csv", ','), loanAppsWriter(basedir + "/loanapps.csv", ','),
     blockedCheckingWriter(basedir + "/blocked_checking.csv", ','), blockedTransferWriter(basedir + "/blocked_transfer.csv", ','),
     blockedTransferSample(100, generator), blockedCheckingSample(100, generator), config(hybench::Config::instantiate(scaleFactor)), generator(config, 0)
// Constructor
{
   auto blocked = (config.customerNumber + config.companyNumber) * config.probBlocked;
   for (int i = 0; i < blocked; i++) {
      int id = generator.generateCustomerOrCompanyId();
      blockedIds.emplace(id);
   }
}
//---------------------------------------------------------------------------
void HyBenchGenerator::generateCustomerWithAccounts()
// Generate the customer table
{
   ProgressPrinter progress("generating customers", config.customerNumber);

   for (int customer = 0; customer < config.customerNumber; ++customer) {
      auto gender = generator.generateGender();
      auto name = generator.generateName(gender);
      auto companyId = generator.generateCompanyId();
      auto age = generator.generateAge();
      auto phone = generator.generatePhone();
      auto provice = generator.generateProvince();
      auto city = generator.generateCity(provice);
      auto date = generator.generateDateBeforeMidPoint();
      auto blocked = blockedIds.contains(customer);

      customerWriter << (customer + 1); // CUSTKEY
      customerWriter << companyId; // COMPANYKEY
      customerWriter << gender; // GENDER
      customerWriter << name; // NAME
      customerWriter << age; // AGE
      customerWriter << phone; // PHONE
      customerWriter << provice; // PROVINCE
      customerWriter << city; // CITY
      customerWriter << CsvWriter::setprecision(2) << config.customerLoanBalance; // LOAN_BALANCE
      customerWriter << 0; // SAVING_CREDIT
      customerWriter << 0; // CHECKING_CREDIT
      customerWriter << 0; // LOAN_CREDIT
      customerWriter << blocked; // ISBLOCKED
      customerWriter << date; // CREATED_DATE
      customerWriter << CsvWriter::endl;

      savingAccountWriter << (customer + 1); // ACCOUNTID
      savingAccountWriter << (customer + 1); // USERID
      savingAccountWriter << CsvWriter::setprecision(2) << config.customerSavingBalance; // BALANCE
      savingAccountWriter << blocked; // ISBLOCKED
      savingAccountWriter << date; // TIMESTAMP
      savingAccountWriter << CsvWriter::endl;

      checkingAccountWriter << (customer + 1); // ACCOUNTID
      checkingAccountWriter << (customer + 1); // USERID
      checkingAccountWriter << CsvWriter::setprecision(2) << config.customerCheckingBalance; // BALANCE
      checkingAccountWriter << blocked; // ISBLOCKED
      checkingAccountWriter << date; // TIMESTAMP
      checkingAccountWriter << CsvWriter::endl;

      progress.update(customer + 1);
   }
}
//---------------------------------------------------------------------------
void HyBenchGenerator::generateCompanyWithAccounts()
// Generate the company table
{
   ProgressPrinter progress("generating companies", config.companyNumber);
   for (int company = config.customerNumber; company < config.customerNumber + config.companyNumber; ++company) {
      auto category = generator.generateCompanyCategory();
      auto name = category + to_string(company + 1) + to_string(hash<string>()(category));
      auto staff = generator.generateCompanyStaff();
      auto phone = generator.generatePhone();
      auto provice = generator.generateProvince();
      auto city = generator.generateCity(provice);
      auto date = generator.generateDateBeforeMidPoint();
      auto blocked = blockedIds.contains(company);

      companyWriter << (company + 1); // COMPANYKEY
      companyWriter << name; // NAME
      companyWriter << category; // CATEGORY
      companyWriter << staff; // STAFF_SIZE
      companyWriter << CsvWriter::setprecision(2) << config.companyLoanBalance; // LOAN_BALANCE
      companyWriter << phone; // PHONE
      companyWriter << provice; // PROVINCE
      companyWriter << city; // CITY
      companyWriter << 0; // SAVING_CREDIT
      companyWriter << 0; // CHECKING_CREDIT
      companyWriter << 0; // LOAN_CREDIT
      companyWriter << blocked; // ISBLOCKED
      companyWriter << date; // CREATED_DATE
      companyWriter << CsvWriter::endl;

      savingAccountWriter << (company + 1); // ACCOUNTID
      savingAccountWriter << (company + 1); // USERID
      savingAccountWriter << CsvWriter::setprecision(2) << config.companySavingBalance; // BALANCE
      savingAccountWriter << blocked; // ISBLOCKED
      savingAccountWriter << date; // TIMESTAMP
      savingAccountWriter << CsvWriter::endl;

      checkingAccountWriter << (company + 1); // ACCOUNTID
      checkingAccountWriter << (company + 1); // USERID
      checkingAccountWriter << CsvWriter::setprecision(2) << config.companyCheckingBalance; // BALANCE
      checkingAccountWriter << blocked; // ISBLOCKED
      checkingAccountWriter << date; // TIMESTAMP
      checkingAccountWriter << CsvWriter::endl;

      progress.update(company - config.customerNumber + 1);
   }
}
//---------------------------------------------------------------------------
void HyBenchGenerator::generateTransfer()
// Generate the transfer table
{
   uint64_t transferCustNumber = config.transferNumber * config.custRate;

   ProgressPrinter progress("generating transfers", config.transferNumber);
   for (uint64_t transfer = 0; transfer < transferCustNumber; ++transfer) {
      int sender = 0;
      int receiver = 0;
      while (sender == receiver) {
         sender = generator.generateCustomerId();
         receiver = generator.generateCustomerOrCompanyId();
      }

      if (blockedIds.contains(receiver)) {
         blockedTransferSample.add(sender);
      }

      double amount = generator.generateReal(0, config.customerSavingBalance * 0.01);
      auto date = generator.generateDateAfterMidPoint();
      auto type = generator.generateCustomerTransferType();

      transferWriter << (transfer + 1); // ID
      transferWriter << sender; // SOURCEID
      transferWriter << receiver; // TARGETID
      transferWriter << CsvWriter::setprecision(2) << amount; // AMOUNT
      transferWriter << type; // TYPE
      transferWriter << date; // TIMESTAMP
      transferWriter << CsvWriter::endl;

      progress.update(transfer + 1);
   }

   for (uint64_t transfer = transferCustNumber; transfer < config.transferNumber; ++transfer) {
      int sender = 0;
      int receiver = 0;
      while (sender == receiver) {
         sender = generator.generateCompanyId();
         receiver = generator.generateCustomerOrCompanyId();
      }

      if (blockedIds.contains(receiver)) {
         blockedTransferSample.add(sender);
      }

      double amount = generator.generateReal(0, config.customerSavingBalance * 0.01);
      auto date = generator.generateDateBeforeMidPoint();
      auto type = generator.generateCompanyTransferType();

      transferWriter << (transfer + 1); // ID
      transferWriter << sender; // SOURCEID
      transferWriter << receiver; // TARGETID
      transferWriter << CsvWriter::setprecision(2) << amount; // AMOUNT
      transferWriter << type; // TYPE
      transferWriter << date; // TIMESTAMP
      transferWriter << CsvWriter::endl;

      progress.update(transfer + 1);
   }

   for (auto id : blockedTransferSample.get()) {
      blockedTransferWriter << id;
      blockedTransferWriter << CsvWriter::endl;
   }
}
//---------------------------------------------------------------------------
void HyBenchGenerator::generateChecking()
// Generate the checking table
{
   uint64_t checkingCustNumber = config.checkingNumber * (1 - config.custRate);

   ProgressPrinter progress("generating checkings", config.checkingNumber);
   for (uint64_t checking = 0; checking < checkingCustNumber; ++checking) {
      int sender = 0;
      int receiver = 0;
      while (sender == receiver) {
         sender = generator.generateCustomerId();
         receiver = generator.generateCustomerOrCompanyId();
      }

      if (blockedIds.contains(receiver)) {
         blockedCheckingSample.add(sender);
      }

      double amount = generator.generateReal(0, config.customerCheckingBalance * 0.01);
      auto date = generator.generateDateAfterMidPoint();
      auto type = generator.generateCustomerCheckingType();

      checkingWriter << (checking + 1); // ID
      checkingWriter << sender; // SOURCEID
      checkingWriter << receiver; // TARGETID
      checkingWriter << CsvWriter::setprecision(2) << amount; // AMOUNT
      checkingWriter << type; // TYPE
      checkingWriter << date; // TIMESTAMP
      checkingWriter << CsvWriter::endl;
   }

   for (uint64_t checking = checkingCustNumber; checking < config.checkingNumber; ++checking) {
      int sender = 0;
      int receiver = 0;
      while (sender == receiver) {
         sender = generator.generateCompanyId();
         receiver = generator.generateCustomerOrCompanyId();
      }

      if (blockedIds.contains(receiver)) {
         blockedCheckingSample.add(sender);
      }

      double amount = generator.generateReal(0, config.customerSavingBalance * 0.01);
      auto date = generator.generateDateAfterMidPoint();
      auto type = generator.generateCompanyCheckingType();

      checkingWriter << (checking + 1); // ID
      checkingWriter << sender; // SOURCEID
      checkingWriter << receiver; // TARGETID
      checkingWriter << CsvWriter::setprecision(2) << amount; // AMOUNT
      checkingWriter << type; // TYPE
      checkingWriter << date; // TIMESTAMP
      checkingWriter << CsvWriter::endl;

      progress.update(checking + 1);
   }

   for (auto id : blockedCheckingSample.get()) {
      blockedCheckingWriter << id;
      blockedCheckingWriter << CsvWriter::endl;
   }
}
//---------------------------------------------------------------------------
void HyBenchGenerator::generateLoan()
// Generate the loan table
{
   ProgressPrinter progress("generating loans", config.loansNumber);

   uint64_t loanCustNumber = config.loanappNumber * (1 - config.custRate);
   for (uint64_t loan = 0; loan < loanCustNumber; ++loan) {
      auto customer = generator.generateCustomerId();
      double amount = generator.generateReal(0, config.customerLoanBalance * config.loanRate);
      auto date = generator.generateLoanDate();
      auto duration = generator.generateLoanDuration();
      auto status = generator.generateLoanStatus();

      loanAppsWriter << (loan + 1); // ID
      loanAppsWriter << customer; // APPLICATIONID
      loanAppsWriter << CsvWriter::setprecision(2) << amount; // AMOUNT
      loanAppsWriter << duration; // DURATION
      loanAppsWriter << status; // STATUS
      loanAppsWriter << date; // TIMESTAMP
      loanAppsWriter << CsvWriter::endl;

      auto oneDayAfter = date + 24h;
      loanTransWriter << (loan + 1); // ID
      loanTransWriter << customer; // APPLICATIONID
      loanTransWriter << (loan + 1); // APPID
      loanTransWriter << CsvWriter::setprecision(2) << amount; // AMOUNT
      loanTransWriter << status; // STATUS
      loanTransWriter << oneDayAfter; // TIMESTAMP
      loanTransWriter << duration; // DURATION
      if (status == "accept")
         loanTransWriter << oneDayAfter; // CONTRACT_TIMESTAMP
      else
         loanTransWriter << "null"; // CONTRACT_TIMESTAMP
      loanTransWriter << 0; // DELINQUENCY
      loanTransWriter << CsvWriter::endl;

      progress.update(loan + 1);
   }

   for (uint64_t loan = loanCustNumber; loan < config.loansNumber; ++loan) {
      auto company = generator.generateRand(config.customerNumber + 1, config.customerNumber + config.companyNumber);
      double amount = generator.generateReal(0, config.companyLoanBalance * config.loanRate);
      auto date = generator.generateLoanDate();
      auto duration = generator.generateLoanDuration();
      auto status = generator.generateLoanStatus();

      loanAppsWriter << (loan + 1); // ID
      loanAppsWriter << company; // APPLICATIONID
      loanAppsWriter << CsvWriter::setprecision(2) << amount; // AMOUNT
      loanAppsWriter << duration; // DURATION
      loanAppsWriter << status; // STATUS
      loanAppsWriter << date; // TIMESTAMP
      loanAppsWriter << CsvWriter::endl;

      auto oneDayAfter = date + 24h;
      loanTransWriter << (loan + 1); // ID
      loanTransWriter << company; // APPLICATIONID
      loanTransWriter << (loan + 1); // APPID
      loanTransWriter << CsvWriter::setprecision(2) << amount; // AMOUNT
      loanTransWriter << status; // STATUS
      loanTransWriter << oneDayAfter; // TIMESTAMP
      loanTransWriter << duration; // DURATION
      if (status == "accept")
         loanTransWriter << oneDayAfter; // CONTRACT_TIMESTAMP
      else
         loanTransWriter << "null"; // CONTRACT_TIMESTAMP
      loanTransWriter << 0; // DELINQUENCY
      loanTransWriter << CsvWriter::endl;

      progress.update(loan + 1);
   }
}
//---------------------------------------------------------------------------
void HyBenchGenerator::generate()
// Generate data
{
   generateCustomerWithAccounts();
   generateCompanyWithAccounts();
   generateTransfer();
   generateChecking();
   generateLoan();
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
