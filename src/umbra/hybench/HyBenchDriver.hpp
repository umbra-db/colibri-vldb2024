#pragma once
//---------------------------------------------------------------------------
#include "umbra/UmbraDriver.hpp"
#include <deque>
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::hybench {
class Generator;
}
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// The shared state beetweeen the clients
struct HyBenchSharedState {
   /// The next transfer id
   std::atomic<uint64_t> transferId = 0;
   /// The next loanapp id
   std::atomic<uint64_t> loanappId = 0;
   /// The next loantrans id
   std::atomic<uint64_t> loantransId = 0;
   /// The next checking id
   std::atomic<uint64_t> checkingId = 0;

   /// The blocked account ids
   std::deque<int> blockedAccountIds;
   /// The blocked transfer ids
   std::vector<int> blockedTransferIds;
   /// The blocked checking ids
   std::vector<int> blockedCheckingIds;

   /// Mutes to protect the blocked account ids
   std::mutex mutex;

   /// Get an blocked account
   int getBlockedAccountId(hybench::Generator& generator);
   /// Get an blocked transfer id
   int getBlockedTransferId(hybench::Generator& generator);
   /// Get an blocked checking id
   int getBlockedCheckingId(hybench::Generator& generator);

   /// Add some blocked accounts
   void addBlockedAccounts(const std::vector<int>& blockedAccounts);
};
//---------------------------------------------------------------------------
/// TPCH driver
class HyBenchDriver : public UmbraDriver {
   /// The modes of the driver
   enum Mode { OLTP,
               OLAP,
               OLXP };

   /// The mode
   Mode mode;
   /// The original log file
   std::string originalLogfile;

   /// The shared state
   HyBenchSharedState state;

   protected:
   /// Create a client
   std::unique_ptr<DatabaseClient> createClient(unsigned int clientIndex) override;

   /// Monitor the driver
   void monitor() override;

   public:
   /// Constructor
   HyBenchDriver(Database& database, std::unique_ptr<DriverConfig> driverConfig, std::string logfile);

   /// Run the driver
   bool run(bool wait) override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
