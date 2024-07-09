#include "postgres/hybench/HyBenchDriver.hpp"
#include "postgres/hybench/HyBenchOLAPClient.hpp"
#include "postgres/hybench/HyBenchOLTPClient.hpp"
#include "postgres/hybench/HyBenchQueryClient.hpp"
#include "postgres/hybench/HyBenchRefreshClient.hpp"
#include "txbench/Database.hpp"
#include "txbench/Exception.hpp"
#include <fstream>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
int HyBenchSharedState::getBlockedAccountId(hybench::Generator& generator)
// Get a blocked account id
{
   unique_lock lock(mutex);
   if (blockedAccountIds.empty())
      return generator.generateCompanyId();

   int res = blockedAccountIds.front();
   blockedAccountIds.pop_front();
   return res;
}
//---------------------------------------------------------------------------
int HyBenchSharedState::getBlockedTransferId(hybench::Generator& generator)
// Get a blocked transfer id
{
   return blockedTransferIds[generator.generateRand(0ul, blockedTransferIds.size() - 1)];
}
//---------------------------------------------------------------------------
int HyBenchSharedState::getBlockedCheckingId(hybench::Generator& generator)
// Get a blocked checking id
{
   return blockedCheckingIds[generator.generateRand(0ul, blockedCheckingIds.size() - 1)];
}
//---------------------------------------------------------------------------
void HyBenchSharedState::addBlockedAccounts(const std::vector<int>& blockedAccounts)
// Add some blocked accounts
{
   unique_lock lock(mutex);
   for (auto blockedAccount : blockedAccounts)
      blockedAccountIds.push_back(blockedAccount);
}
//---------------------------------------------------------------------------
HyBenchDriver::HyBenchDriver(Database& database, unique_ptr<DriverConfig> driverConfig, string logfile)
   : PostgresDriver(database, std::move(driverConfig), std::move(logfile)), mode(OLAP), originalLogfile(this->logfile)
// Constructor
{
   hybench::Config config(hybench::Config::instantiate(database.getScaleFactor()));
   state.transferId = config.transferNumber;
   state.loanappId = config.loanappNumber;
   state.loantransId = config.loansNumber;
   state.checkingId = config.checkingNumber;
}
//---------------------------------------------------------------------------
unique_ptr<DatabaseClient> HyBenchDriver::createClient(unsigned clientIndex)
// Create a client
{
   auto transactionalClients = database.getTransactionalClients();
   if (clientIndex < transactionalClients) {
      if (mode == OLTP) {
         return make_unique<HyBenchOLTPClient>(database, state, clientIndex + 1);
      } else if (mode == OLXP) {
         return make_unique<HyBenchRefreshClient>(database, state, clientIndex + 1);
      } else {
         return nullptr;
      }
   } else {
      if (mode == OLAP) {
         return make_unique<HyBenchOLAPClient>(database, clientIndex + 1);
      } else if (mode == OLXP) {
         return make_unique<HyBenchQueryClient>(database, state, clientIndex + 1);
      } else {
         return nullptr;
      }
   }
}
//---------------------------------------------------------------------------
void HyBenchDriver::monitor()
// Monitor the driver
{
   monitorTransactionsAndQueries();
}
//---------------------------------------------------------------------------
bool HyBenchDriver::run(bool wait)
// Run the driver
{
   auto readBlockedIds = [](std::vector<int>& ids, const string& filename) {
      ifstream file(filename);
      if (!file.is_open()) {
         throw Exception("Could not open file " + filename);
      }

      string line;
      while (getline(file, line)) {
         ids.push_back(stoi(line));
      }
   };

   readBlockedIds(state.blockedTransferIds, "/data/blocked_transfer.csv");
   readBlockedIds(state.blockedCheckingIds, "/data/blocked_checking.csv");

   if (database.getAnalyticalClients() != 0) {
      mode = OLAP;
      logfile = originalLogfile + "_olap";
      clients.clear();
      if (!PostgresDriver::run(true)) return false;
   }

   if (database.getTransactionalClients() != 0) {
      mode = OLTP;
      logfile = originalLogfile + "_oltp";
      clients.clear();
      if (!PostgresDriver::run(true)) return false;
   }

   mode = OLXP;
   logfile = originalLogfile + "_olxp";
   clients.clear();
   return PostgresDriver::run(wait);
}
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
