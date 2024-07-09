#pragma once
//---------------------------------------------------------------------------
#include "txbench/Database.hpp"
#include "txbench/pq/PqConnection.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::postgres {
//---------------------------------------------------------------------------
/// Microsoft SQL server
class Postgres : public Database {
   protected:
   /// The postgres connection
   pq::PqConnection connection;

   /// Prepare the connection config
   std::unique_ptr<ConnectionConfig> prepareConnectionConfig() const override;
   /// Prepare the database driver
   std::unique_ptr<DatabaseDriver> prepareDriver(const std::string& benchmarkConfigFile, const std::string& logfile) override;

   public:
   /// Constructor
   Postgres(System database, Benchmark benchmark, std::filesystem::path scriptsPath, std::filesystem::path logPath, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients);
   /// Destructor
   ~Postgres() = default;

   /// Execute a query
   void executeQuery(const std::string& query) override;

   /// Connect to the database
   void connect() override;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
