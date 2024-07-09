#pragma once
//---------------------------------------------------------------------------
#include "txbench/Database.hpp"
#include "txbench/odbc/Connection.hpp"
#include "txbench/odbc/Environment.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::singlestore {
//---------------------------------------------------------------------------
/// Microsoft SQL server
class SingleStore : public Database {
   /// The odbc environment
   odbc::Environment environment;
   /// The odbc connection
   odbc::Connection connection;

   protected:
   /// Prepare the connection config
   std::unique_ptr<ConnectionConfig> prepareConnectionConfig() const override;
   /// Prepare the database driver
   std::unique_ptr<DatabaseDriver> prepareDriver(const std::string& driverConfigFile, const std::string& logfile) override;

   public:
   /// Constructor
   SingleStore(System database, Benchmark benchmark, std::filesystem::path scriptsPath, std::filesystem::path logPath, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients);
   /// Destructor
   ~SingleStore() = default;

   /// Execute a query
   void executeQuery(const std::string& query) override;

   /// Connect to the database
   void connect() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
