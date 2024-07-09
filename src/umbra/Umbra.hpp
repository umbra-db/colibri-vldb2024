#pragma once
//---------------------------------------------------------------------------
#include "postgres/Postgres.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::umbra {
//---------------------------------------------------------------------------
/// Microsoft SQL server
class Umbra : public postgres::Postgres {
   /// The pid of the local umbra process
   pid_t umbraPid = 0;

   protected:
   /// Prepare the connection config
   std::unique_ptr<ConnectionConfig> prepareConnectionConfig() const override;
   /// Prepare the database driver
   std::unique_ptr<DatabaseDriver> prepareDriver(const std::string& benchmarkConfigFile, const std::string& logfile) override;

   /// Start the database
   void startDatabase() override;
   /// Shutdown the database
   void shutdownDatabase() override;

   public:
   /// Constructor
   Umbra(System database, Benchmark benchmark, std::filesystem::path scriptsPath, std::filesystem::path logPath, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients);
   /// Destructor
   ~Umbra();

   /// Execute a benchmark
   void execute(const std::string& driverConfigFile) override;

   /// Check whether the umbra should be traced
   static bool trace();
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
