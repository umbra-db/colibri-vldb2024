#pragma once
//---------------------------------------------------------------------------
#include "txbench/Database.hpp"
//---------------------------------------------------------------------------
// Copyright (c) 2022 TUM. All rights reserved.
//---------------------------------------------------------------------------
namespace txbench::duckdb {
//---------------------------------------------------------------------------
class DuckDBConnection;
class DuckDBDatabase;
//---------------------------------------------------------------------------
/// Duck DB
class DuckDB : public Database {
   /// The database
   std::unique_ptr<DuckDBDatabase> database;
   /// The connection
   std::unique_ptr<DuckDBConnection> connection;

   protected:
   /// Prepare the connection config
   std::unique_ptr<ConnectionConfig> prepareConnectionConfig() const override;
   /// Prepare the database driver
   std::unique_ptr<DatabaseDriver> prepareDriver(const std::string& driverConfigFile, const std::string& logfile) override;

   public:
   /// Constructor
   DuckDB(System database, Benchmark benchmark, std::filesystem::path scriptsPath, std::filesystem::path logPath, uint64_t scaleFactor, unsigned transactionalClients, unsigned analyticalClients);
   /// Destructor
   ~DuckDB();

   /// Execute a query
   void executeQuery(const std::string& query) override;

   /// Connect to the database
   void connect() override;

   /// Open connection
   std::unique_ptr<DuckDBConnection> openConnection() const;
};
//---------------------------------------------------------------------------
}
//---------------------------------------------------------------------------
